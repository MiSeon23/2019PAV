#include "Servo_Infinite.h"
#include "mbed.h"

Servo_Infinite::Servo_Infinite(PinName pin) : ServoPwm(pin) {}


Servo_Infinite& Servo_Infinite::operator= (int srv_dutyCycle) { 
    ServoPwm.pulsewidth_us(srv_dutyCycle);
    return *this;
}