#ifndef FOLDING_H
#define FOLDING_H

#include "mbed.h"
#include "Servo_Infinite.h"
#include "ServoOP.h"

class Folding {

public:
    void Folding_init ();
    void Only_sig ();
    void Folding_func ();
    void Unfolding_func ();
    void None ();
    void Flag_Detect ( int );
    void Folding_run ();
    void Unfolding_run ();
    void None_run ();
    void Flight_run ();

protected:
};

#endif