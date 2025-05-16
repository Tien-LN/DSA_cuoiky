#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm> 
#include <cctype>    
#include "file_handler.hpp"
#include "student.hpp"
#include "utils.hpp"

#define WIN32_LEAN_AND_MEAN // Add this to reduce the scope of windows.h
#include <windows.h> // Required for SetConsoleTextAttribute

using namespace std;

const WORD FH_BRIGHT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const WORD FH_BRIGHT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY;
const WORD FH_BRIGHT_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; 
const WORD BRIGHT_PURPLE_MAIN = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; 
const WORD FH_DEFAULT_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; 

bool importDataFromFile(const string& filename, vector<Student>& students, bool& hasInvalidRows) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Get console handle

    students.clear();
    
    SetConsoleTextAttribute(hConsole, FH_BRIGHT_YELLOW);
    cout << "Đang tải dữ liệu từ file: " << filename << endl << endl;
    SetConsoleTextAttribute(hConsole, FH_DEFAULT_COLOR);

    // lấy đuôi file
    string extension;
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != string::npos) {
        extension = filename.substr(dotPos);
    }

    // chỉ hỗ trợ file csv
    if (extension != ".csv") {
        SetConsoleTextAttribute(hConsole, FH_BRIGHT_RED);
        cout << "Chỉ hỗ trợ file CSV. Vui lòng thử lại." << endl;
        SetConsoleTextAttribute(hConsole, FH_DEFAULT_COLOR);
        return false;
    } else {
        ifstream file(filename);
        if (!file.is_open()) {
            SetConsoleTextAttribute(hConsole, FH_BRIGHT_RED);
            cout << "Không thể mở file: " << filename << endl;
            SetConsoleTextAttribute(hConsole, FH_DEFAULT_COLOR);
            return false;
        }

        string line;
        // Bỏ qua dòng tiêu đề file, kiểm tra xem có đọc được không
        if (!getline(file, line)) {
            SetConsoleTextAttribute(hConsole, FH_BRIGHT_YELLOW);
            cout << "File rỗng hoặc không thể đọc dòng tiêu đề." << endl;
            SetConsoleTextAttribute(hConsole, FH_DEFAULT_COLOR);
            file.close();
            return true; 
        }

        set<string> mssvList; 

        int lineNumber = 1; // Bắt đầu từ dòng 2 vì dòng 1 là tiêu đề

        while (getline(file, line)) {
            lineNumber++;
            stringstream ss(line);
            string mssv, ho, ten, lop, diemStr;
            vector<string> errorMessages;
            float diem = 0.0f;

            // đọc từng trường dữ liệu
            getline(ss, mssv, ',');
            getline(ss, ho, ',');
            getline(ss, ten, ',');
            getline(ss, lop, ',');
            getline(ss, diemStr, ',');

            // xử lí dữ liệu (loại bỏ khoảng trắng đầu cuối)
            mssv = trim(mssv);
            // chuyển đổi mssv về chữ in
            transform(mssv.begin(), mssv.end(), mssv.begin(), [](unsigned char c){ return toupper(c); });
            ho = trim(ho);
            ten = trim(ten);
            lop = trim(lop);
            diemStr = trim(diemStr);

            // --- Bắt đầu kiểm tra dữ liệu --- 
            // Sử dụng các hàm validation từ utils.cpp
            bool mssvValid = validateMSSV(mssv, mssvList, errorMessages);
            bool hoValid = validateHo(ho, errorMessages);
            bool tenValid = validateTen(ten, errorMessages);
            bool lopValid = validateLop(lop, errorMessages);
            bool diemValid = validateDiem(diemStr, diem, errorMessages);

            bool isValidStudent = mssvValid && hoValid && tenValid && lopValid && diemValid;
            // --- Kết thúc kiểm tra dữ liệu ---
            if (isValidStudent) {
                students.emplace_back(mssv, ho, ten, lop, diem);
                mssvList.insert(mssv); 
            } else {
                hasInvalidRows = true;
                SetConsoleTextAttribute(hConsole, FH_BRIGHT_RED);
                cout << "Lỗi dòng " << lineNumber << ": \n"; 
                SetConsoleTextAttribute(hConsole, FH_BRIGHT_YELLOW);
                cout << "\tNội dung dòng: \"" << line << "\"." << endl; 
                SetConsoleTextAttribute(hConsole, BRIGHT_PURPLE_MAIN);
                cout << "\tLý do: ";
                SetConsoleTextAttribute(hConsole, FH_DEFAULT_COLOR);
                for (size_t i = 0; i < errorMessages.size(); ++i) {
                    cout << "\n\t\t";
                    cout << errorMessages[i] << (i == errorMessages.size() - 1 ? "" : "; ");
                }
                cout << endl << endl;
            }
        }
        file.close(); 
        return true;
    }
    return false; 
}