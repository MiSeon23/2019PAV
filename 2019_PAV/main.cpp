#include "mbed.h"
#include "Basic.h"

#define Driving_Mode    0
#define Flight_Mode     1

struct Folding_flag{
    int state_folding;
    int state_none;
    int state_unfolding;
};

Folding_flag F_flag = {352, 1024, 1696};

DigitalOut InitCheck_Led(LED1);
DigitalOut ModeCheck_Led(LED2); //Only Check Driving, Flight Mode

Serial pc(USBTX, USBRX);

int Mode = Driving_Mode;
int F_Flag_check = 0;
bool sbus_Mode = false;

//Execution Function Import
Driving driving;
Folding folding;

//For Communication
FutabaSBUS sbus(p13, p14);

//----Scheduler----
int flag_20ms=0,flag_50ms=0;

void counter_1ms() {
    flag_20ms++;
    flag_50ms++;
}
//-----Don't Change----


int main()
{
    InitCheck_Led = 0;
    driving.Driving_init();
    folding.Folding_init();
    
    for( int i = 0; i<100; i++ ){}  //sbus signal이 처음에 잠깐 튀는 것을 방지
    
    wait(1);
    InitCheck_Led = 1;
    
    //----Scheduler---
    Ticker ticker_1ms;
    ticker_1ms.attach(&counter_1ms,0.001);
    //---Don't Change---
    
    while(1) {
    
        if ( Mode == Driving_Mode ) {
//            if(sbus_Mode == false) sbus.ticker(500); //printf 디버깅 끝나면 500으로 원상복귀하기
//            sbus_Mode = true;
            ModeCheck_Led = 0;
            
            // Every 20 ms,
            if ( flag_20ms >= 20 ) {
                driving.Drivig_run(sbus.channel(4),sbus.channel(2)); //steer, motor 순
                flag_20ms=0;
            }
        
            // Every 50 ms,
            if ( flag_50ms >= 50 ) {
                F_Flag_check = sbus.channel(6);
                folding.Flag_Detect( F_Flag_check );
                if      ( F_Flag_check == F_flag.state_folding )      folding.Folding_run();
                else if ( F_Flag_check == F_flag.state_none )         folding.None_run();
                else if ( F_Flag_check == F_flag.state_unfolding )    folding.Unfolding_run();
                flag_50ms=0;
            }
            Mode = ( sbus.channel(10) - 352 )/1344;
        }//Driving_Mode
        
        else if ( Mode == Flight_Mode ){
//            if(sbus_Mode == true) sbus.ticker(500);
//            sbus_Mode = false;
            ModeCheck_Led = 1;
            
            if ( flag_20ms >= 20 ) {
                folding.Flight_run();
                flag_20ms=0;
            }
        
            // Every 50 ms,
            if ( flag_50ms >= 50 ) {
                flag_50ms=0;
            }
            Mode = ( sbus.channel(10) - 352 )/1344;
        }//Flight_Mode
    }//while
}//main