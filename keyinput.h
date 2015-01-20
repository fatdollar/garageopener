#ifndef _KEYINPUT_H_
#define _KEYINPUT_H_

#include <Arduino.h>
#include "..\\libraries\\Keypad\\Keypad.h"

#define LIST_MAX 2

class KeyInput
{
private:
    //constants
    static const byte ROWS = 4, COLS = 3;
    static char keys[ROWS][COLS];
    static byte rowPins[ROWS];
    static byte colPins[COLS];

    //variables
    Keypad *kypd;

public:
    //functions
    KeyInput(char keymap[][COLS]=keys, byte row[ROWS]=rowPins, byte col[COLS]=colPins, byte rows=ROWS, byte cols=COLS);
    bool getKeys();
    Key key(byte num);
};


#endif
