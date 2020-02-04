#include "Servo.h"
#include "mbed.h"

static float clamp(float value, float min, float max) {
  if(value < min) {
        return min;
    } else if(value > max) {
        return max;
    } else {
        return value;
    }
}

Servo::Servo(PinName pin) : ServoPwm(pin) {
    calibrate();
    write(0.5);
}

void Servo::write(float srv_angle) { //srv_angle = -90.0 ~ 90.0
    float NewPulsewidth = 10.7*srv_angle + 1500; // 537 ~ 1500 ~ 2463
    ServoPwm.pulsewidth_us(clamp(NewPulsewidth, PulseMin, PulseMax));
}

void Servo::calibrate(float range) {
    PulseMin = 1500 - range;
    PulseMax = 1500 + range;
}

Servo& Servo::operator= (float srv_angle) { 
    write(srv_angle);
    return *this;
}