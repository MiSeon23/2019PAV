#include "Folding.h"
#include "mbed.h"

/* ---- Speed Control ----             */
/* 663/638 미만 작을수록 빠름 : LEFT     */
/* 663/638 미만 근접할수록 느림 : LEFT   */
/* 663/638 초과 클수록 빠름 : RIGHT      */
/* 663/638 초과 근접할수록 느림 : RIGHT  */

Serial PC(USBTX, USBRX);

Servo_Infinite Back_2_State(p22);
Servo_Infinite Right_State(p21);
Servo_Infinite Left_State(p23);

ServoOpen Front_2_State(p20);
ServoOpen Back_1_State(p19);
ServoOpen Front_1_State(p17);

bool folding_flag = false;
bool unfolding_flag = false;
int count_folding = 0;
int count_unfolding = 0;
int temp_check = 352;

struct Servo_State{
    int Stop;
    int Right_Fast;
    int Right_Slow;
    int Left_Fast;
    int Left_Slow;
};
Servo_State Front_1 = {701, 2500, 700, 1, 620};   //p17 638, 2500, 680, 1, 600
Servo_State Back_1 = {700, 2500, 750, 1, 670};    //p19
Servo_State Front_2 = {686, 2500, 760, 1, 650};   //p20
Servo_State Back_2 = {0, 2500, 780, 1, 670};      //p22
Servo_State Right = {0, 2500, 760, 1, 650};       //p21
Servo_State Left = {0, 2500, 780, 1, 670};        //p23

//---!! 함수에다 wait걸지 않기 !!---//

//초기화
void Folding::Folding_init () {
    Back_2_State = Back_2.Stop;   Right_State = Right.Stop;   Left_State = Left.Stop;
}

void Folding::Folding_func () {    
    
    count_folding++;
  
    Front_2_State.Enable( Right.Right_Fast, 20000 );
    Back_1_State.Enable( Back_1.Left_Fast, 20000 );  
    Front_1_State.Enable( Front_1.Left_Fast, 20000 );
    
// ----- for 3 steps ----
    if( folding_flag ) { //FRONTS -> LEFT/RIGHT
        if( count_folding <= 30 ) {   
            Front_2_State.SetPosition( Front_2.Stop );
            Front_1_State.SetPosition( Front_1.Stop );
            Left_State = Left.Right_Fast;
            Right_State = Right.Right_Fast;
            Back_2_State = Back_2.Right_Fast;
            Back_1_State.SetPosition( Back_1.Right_Fast );
        }
        else if( (30 < count_folding)  && (count_folding <= 82) ) {
            Front_2_State.SetPosition( Front_2.Right_Fast );
            Front_1_State.SetPosition( Front_1.Right_Fast );
            Left_State = Left.Right_Fast;
            Right_State = Right.Right_Fast;
            Back_2_State = Back_2.Right_Fast;
            Back_1_State.SetPosition( Back_1.Right_Fast );

            if ( count_folding >= 81)       Front_2_State.SetPosition( Front_2.Stop );
            if ( count_folding >= 81)       Front_1_State.SetPosition( Front_1.Stop );
            if ( count_folding >= 53)       Left_State = Left.Stop;
            if ( count_folding >= 62)       Right_State = Right.Stop;
            if ( count_folding >= 60)       Back_2_State = Back_2.Stop;
            if ( count_folding >= 71)       Back_1_State.Disable();//Back_1_State = Back_1.Stop;
        }      
        else if( count_folding >= 81 ) {
            folding_flag = false;
        }
    }
    else None();
//    PC.printf("%d ,folding\n\r", count_folding);
}



void Folding::Unfolding_func () {
        
    count_unfolding++;
    
    Front_2_State.Enable( Right.Stop, 20000);
    Back_1_State.Enable( Back_1.Right_Fast, 20000 );  
    Front_1_State.Enable( Front_1.Left_Fast, 20000 );

// ---- for 3 steps ----    
    if( unfolding_flag ) { //LEFT/RIGHT -> FRONTS
        if( count_unfolding <= 30 ) { 
            Front_2_State.SetPosition( Front_2.Left_Fast );
            Front_1_State.SetPosition( Front_1.Left_Fast );  
            Left_State = Left.Stop;
            Right_State = Right.Stop;
            //Right_State.Disable();
            Back_2_State = Back_2.Left_Fast;
            Back_1_State.SetPosition( Back_1.Left_Fast );
        }     
        else if( (30 < count_unfolding)  && (count_unfolding <= 86) ) {
            Front_2_State.SetPosition( Front_2.Left_Fast );
            Front_1_State.SetPosition( Front_1.Left_Fast );
            Left_State = Left.Left_Fast;
            Right_State = Right.Left_Fast;
            Back_2_State = Back_2.Left_Fast;
            Back_1_State.SetPosition( Back_1.Left_Fast );

            if ( count_unfolding >= 83) Left_State = Left.Stop;
            if ( count_unfolding >= 87) Right_State = Right.Stop;
            if ( count_unfolding >= 60) Back_2_State = Back_2.Stop;
            if ( count_unfolding >= 66) Back_1_State.Disable();//Back_1_State.SetPosition( Back_1.Stop );
            if ( count_unfolding >= 52) Front_2_State.SetPosition( Front_2.Stop );
            if ( count_unfolding >= 47 ) Front_1_State.SetPosition( Front_1.Stop );
        }      
        else if( count_unfolding >= 85 ) {
            unfolding_flag = false;
        }
    }
    else None();
//    PC.printf("%d , unfolding\n\r", count_unfolding);
}

void Folding::None () {
    Back_2_State = Back_2.Stop;     Front_2_State.Disable();
    Back_1_State.Disable();         Front_1_State.Disable();
    Left_State = Left.Stop;         Right_State = Right.Stop;
}

void Folding::Flag_Detect ( int F_Flag_check ) {
    if ( temp_check - F_Flag_check > 0 ) {
        folding_flag = true;
        count_folding = 0; }
    else if ( temp_check - F_Flag_check < 0 ) {
        unfolding_flag = true;
        count_unfolding = 0; }
    temp_check = F_Flag_check;
}


//----- main에서 실행될 함수 -----
void Folding::Folding_run () {
    Folding_func();
}

void Folding::Unfolding_run () {
    Unfolding_func();
}

void Folding::None_run () {
    None();
}

void Folding::Flight_run () {
    Back_2_State = Back_1.Stop;     Front_2_State.Disable();
    Back_1_State.Disable();         Front_1_State.Disable();
    Left_State = Left.Stop;         Right_State = Right.Stop;
}