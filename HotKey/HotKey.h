#pragma once

#include <iostream>
#include <Windows.h>


template <typename... Args>
void println(const char* format, Args... args);


const INT HOTKEYID = 1;
const INT QUITKEYID = 2;

// Character 'E'
const INT HOTKEY_VALUE = 0x45; 
// Character 'Q'
const INT QUITKEY_VALUE = 0x51;

void ExecSpam();



