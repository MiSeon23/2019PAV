#include "Driving.h"
#include "mbed.h"

#define PI  3.141592

Serial pc_D(USBTX, USBRX);

Servo Steering_Srv(p26); //Steering Servo Motor
PwmOut InWheel_Left(p25); //Inwheel Motor Left
PwmOut InWheel_Right(p24); //Inwheel Motor Right

float Steer_ang = 0.0;
float Left_vel = 0.0;
float Right_vel = 0.0;

//---!! 함수에다 wait걸지 않기 !!---//

//초기화
void Driving::Driving_init () {
    Steering_Srv = 90.0;     Steer_ang = 0.0;
    InWheel_Left.pulsewidth(0.0015);     Left_vel = 0.0015;
    InWheel_Right.pulsewidth(0.0015);    Right_vel = 0.0015;
}

//main에서 실행될 함수
void Driving::Drivig_run ( int Sbus_Steering_Sig, int Sbus_Motor_Sig ) {
    float Servo_Ang = Sbus_Steering_Sig * 0.1339285714 - 137.1428571328; // -90 ~ 90
     float Motor_Vel = Sbus_Motor_Sig * 0.0000007440476 + 0.0007380;
    //Steering
    Steer_ang = Servo_Ang;  Steering_Srv = Steer_ang;
    Left_vel = (Servo_Ang*0.002222222+0.35)*(Motor_Vel-0.0015)+0.0015;
    Right_vel = (-Servo_Ang*0.002222222+0.35)*(Motor_Vel-0.0015)+0.0015;

    //printf("Servo = %f, Left = %f, Right = %f, Motor = %f\n\r", Servo_Ang, Left_vel, Right_vel, Motor_Vel);
    InWheel_Left.pulsewidth( Left_vel );    InWheel_Right.pulsewidth( Right_vel );
}
