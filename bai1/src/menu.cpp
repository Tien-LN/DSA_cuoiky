#include "menu.hpp"
#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

const WORD BRIGHT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const WORD BRIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD BRIGHT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY;
const WORD BRIGHT_YELLOW_MAIN = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const WORD BRIGHT_PURPLE_MAIN = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; 
const WORD DEFAULT_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; 

void displayMenu(bool dataLoaded, bool hasInvalidRows, bool dataStructureSelected, string dsName) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
    cout << "\n\n--------------------MENU🪄------------------\n\n";
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW_MAIN);
    cout << "1. Tải dữ liệu từ file 💾\n\n";
    if (dataLoaded && !hasInvalidRows) {
        if (!dataStructureSelected) {
            cout << "2. Chọn cấu trúc dữ liệu 🕹️\n\n";
        } else {
            cout << "2. chọn cấu trúc dữ liệu khác 🕹️";
            SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
            cout << "\n   Cấu trúc dữ liệu hiện tại: ";
            SetConsoleTextAttribute(hConsole, BRIGHT_BLUE);
            cout << dsName << endl << endl;
            SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW_MAIN);
            cout << "3. Thêm sinh viên ➕\n\n";
            cout << "4. Cập nhật thông tin sinh viên ✒️\n\n";
            cout << "5. Xóa sinh viên 🗑️\n\n";
            cout << "6. Thống kê sinh viên 🧮\n\n";
            cout << "7. Sắp xếp sinh viên 📏\n\n";
            cout << "8. Tìm kiếm sinh viên 🔍\n\n";
        }
    }

    SetConsoleTextAttribute(hConsole, BRIGHT_RED);
    cout << "0. Thoát\n";

    SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
    cout << "\n--------------------------------------------\n\n";
}