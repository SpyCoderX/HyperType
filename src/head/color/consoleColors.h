#ifndef CONSOLECOLORS_H
#define CONSOLECOLORS_H

#include <windows.h>
#include <string>
#include <iostream>

namespace ConsoleColors {
    enum class ConsoleColor {
        Black = 0,
        Blue = 1,
        Green = 2,
        Cyan = 3,
        Red = 4,
        Magenta = 5,
        Brown = 6,
        LightGray = 7,
        DarkGray = 8,
        LightBlue = 9,
        LightGreen = 10,
        LightCyan = 11,
        LightRed = 12,
        LightMagenta = 13,
        Yellow = 14,
        White = 15
    };
    
    enum class ConsoleBackgroundColor {
        Black = 0,
        Blue = 16,
        Green = 32,
        Cyan = 48,
        Red = 64,
        Magenta = 80,
        Brown = 96,
        LightGray = 112,
        DarkGray = 128,
        LightBlue = 144,
        LightGreen = 160,
        LightCyan = 176,
        LightRed = 192,
        LightMagenta = 208,
        Yellow = 224,
        White = 240
    };
    void SetConsoleColor(ConsoleColor textColor, ConsoleBackgroundColor backgroundColor) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole != INVALID_HANDLE_VALUE) {
            SetConsoleTextAttribute(hConsole, static_cast<WORD>(textColor) | static_cast<WORD>(backgroundColor));
        }
    }
    void ResetConsoleColor() {
        SetConsoleColor(ConsoleColor::LightGray, ConsoleBackgroundColor::Black);
    }
    void SetConsoleColor(ConsoleColor color) {
        SetConsoleColor(color, ConsoleBackgroundColor::Black);
    }
    void PrintError(std::string str) {
        SetConsoleColor(ConsoleColor::LightRed);
        std::cout << str << std::endl;
        ResetConsoleColor();
    }
    void PrintSuccess(std::string str) {
        SetConsoleColor(ConsoleColor::Green);
        std::cout << str << std::endl;
        ResetConsoleColor();
    }
    void PrintError(const char* chars) {
        PrintError(std::string(chars));
    }
    void PrintSuccess(const char* chars) {
        PrintSuccess(std::string(chars));
    }
}

#endif // CONSOLECOLORS_H