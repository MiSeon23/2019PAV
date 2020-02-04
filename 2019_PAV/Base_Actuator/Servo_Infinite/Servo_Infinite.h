#ifndef MBED_SERVO_INFINITE_H
#define MBED_SERVO_INFINITE_H
#include "mbed.h"
#include <PwmOut.h>

class Servo_Infinite {

public:
    Servo_Infinite(PinName pin);
    Servo_Infinite& operator= (int srv_dutyCycle);

protected:
    PwmOut ServoPwm;
};

#endif
