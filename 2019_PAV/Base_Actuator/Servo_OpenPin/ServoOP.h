#ifndef MBED_SERVOOP_H
#define MBED_SERVOOP_H

#include "mbed.h"

class ServoOpen {

public:
    ServoOpen(PinName pin);
    
    void SetPosition(int NewPos);    
    void Enable(int StartPos, int Period);
    void Disable();

private:
    void StartPulse();
    void EndPulse();

    int Position;
    DigitalOut ServoPin;
    Ticker Pulse;
    Timeout PulseStop;
};

#endif