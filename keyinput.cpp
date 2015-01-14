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
    byte rowPins[ROWS] =  {7, 6, 5, 4};
    byte colPins[COLS] = {10, 9, 8};
    kypd = new Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
}

Key *KeyInput::getKeys()
{
    return kypd->key;
}