#ifndef DRIVING_H
#define DRIVING_H

#include "mbed.h"
#include "Servo.h"

class Driving {

public:
    void Driving_init ();
    void Drivig_run (int, int);

protected:
    float Steer_ang;
    float Left_vel;
    float Right_vel;
};

#endif