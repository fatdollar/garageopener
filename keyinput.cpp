#include "keyinput.h"

KeyInput::KeyInput()
{
    char keys[ROWS][COLS] =
    {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
    };
    byte rowPins[ROWS] =  {5, 4, 3, 2};
    byte colPins[COLS] = {8, 7, 6};
    kypd = new Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
}

Key *KeyInput::getKeys()
{
    return kypd->key;
}