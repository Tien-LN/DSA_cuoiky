#include "student.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

string Student::reversedName() const {
    string name = getFullName();
    
    vector<string> characters;
    
    // Tách chuỗi thành các ký tự UTF-8
    for (size_t i = 0; i < name.length(); ) {
        unsigned char c = name[i];
        string character;
        
        if (c < 0x80) {
            // Ký tự ASCII (1 byte)
            character = name.substr(i, 1);
            i += 1;
        } else if ((c & 0xE0) == 0xC0) {
            // Ký tự UTF-8 2 byte
            character = name.substr(i, 2);
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            // Ký tự UTF-8 3 byte
            character = name.substr(i, 3);
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            // Ký tự UTF-8 4 byte
            character = name.substr(i, 4);
            i += 4;
        } else {
            // UTF-8 không hợp lệ, coi như một byte
            character = name.substr(i, 1);
            i += 1;
        }
        
        characters.push_back(character);
    }
    
    // Đảo ngược thứ tự các ký tự
    string reversedString = "";
    for (auto it = characters.rbegin(); it != characters.rend(); ++it) {
        reversedString += *it;
    }
    
    return reversedString;
}
