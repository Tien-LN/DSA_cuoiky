#include "student.hpp"
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

string Student::reversedName() const {
    string name = getFullName();
    reverse(name.begin(), name.end());
    return name;
}
