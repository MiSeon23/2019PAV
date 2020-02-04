#ifndef MBED_SERVO_H
#define MBED_SERVO_H

#include "mbed.h"
#include <PwmOut.h>

class Servo {

public:
    Servo(PinName pin);

    void write(float percent);
    void calibrate(float range = 960); 
        
    Servo& operator= (float srv_angle);

protected:
    PwmOut ServoPwm;
    float PulseMin;
    float PulseMax;
};

#endif
