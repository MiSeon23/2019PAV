#include "ServoOP.h"
#include "mbed.h"

    ServoOpen::ServoOpen(PinName Pin) : ServoPin(Pin) { }

    void ServoOpen::SetPosition(int Pos) {
        Position = Pos;
    }

    void ServoOpen::StartPulse() {
        ServoPin = 1;
        PulseStop.attach_us(this, &ServoOpen::EndPulse, Position);
    }

    void ServoOpen::EndPulse() {
        ServoPin = 0;
    }

    void ServoOpen::Enable(int StartPos, int Period) {
        Position = StartPos;
        Pulse.attach_us(this, &ServoOpen::StartPulse, Period);
    }

    void ServoOpen::Disable() {
        Pulse.detach();
    }