#ifndef _KEYINPUT_H_
#define _KEYINPUT_H_

#include <Arduino.h>
#include "..\\libraries\\Keypad\\Keypad.h"

class KeyInput
{
private:
    //constants
    static const byte ROWS = 4, COLS = 3;

    //variables
    Keypad *kypd;

public:
    //functions
    KeyInput();
    Key *getKeys();
};


#endif
