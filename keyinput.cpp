#include "keyinput.h"

char KeyInput::keys[ROWS][COLS] =
{
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};
byte KeyInput::rowPins[ROWS] = {9, 4, 5, 7};
byte KeyInput::colPins[COLS] = {8, 10, 6};

KeyInput::KeyInput(char keymap[][COLS], byte row[ROWS], byte col[COLS], byte rows, byte cols)
{
    kypd = new Keypad(makeKeymap(keymap), row, col, rows, cols);
}

bool KeyInput::getKeys()
{
    return kypd->getKeys();
}

Key KeyInput::key(byte num)
{
    return kypd->key[num];
}