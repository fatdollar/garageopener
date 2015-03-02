#include "keyinput.h"

byte KeyInput::rowPins[ROWS] = {9, 4, 5, 7};
byte KeyInput::colPins[COLS] = {8, 10, 6};

char KeyInput::keys[ROWS][COLS] =
{
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

////////////////////////////////////////
//KeyInput::KeyInput() - initializes Keypad with defined keys, pins
//
//Inputs: none
//
//Outputs: none
////////////////////////////////////////
KeyInput::KeyInput(char keymap[][COLS], byte row[ROWS], byte col[COLS], byte rows, byte cols)
{
    kypd = new Keypad(makeKeymap(keymap), row, col, rows, cols);
}

////////////////////////////////////////
//KeyInput::getKeys() - returns if keys are active
//
//Inputs: none
//
//Outputs: bool
//			true - active keys found
//			false - no active keys found
////////////////////////////////////////
bool KeyInput::getKeys()
{
    return kypd->getKeys();
}

////////////////////////////////////////
//KeyInput::key() - gets key informations for desired index
//
//Inputs: byte num
//			index of desired key info
//
//Outputs: Key
//			info for key at index num
////////////////////////////////////////
Key KeyInput::key(byte num)
{
    return kypd->key[num];
}