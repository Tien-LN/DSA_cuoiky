#ifndef CONSOLE_UTILS_HPP
#define CONSOLE_UTILS_HPP

// Disable std::byte by defining _HAS_STD_BYTE to 0 before including any standard headers
#define _HAS_STD_BYTE 0

#ifdef _WIN32
#include <windows.h>
#endif

// Color constants for menu items
extern const WORD BRIGHT_GREEN_MAIN;
extern const WORD BRIGHT_BLUE_MAIN;
extern const WORD BRIGHT_RED_MAIN;
extern const WORD BRIGHT_YELLOW;
extern const WORD BRIGHT_VIOLET_MAIN;
extern const WORD BRIGHT_PURPLE_MAIN;
extern const WORD DEFAULT_COLOR_MAIN;

// Declare console handle as extern
extern HANDLE hConsole;

#endif // CONSOLE_UTILS_HPP
