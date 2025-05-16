#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <vector>
#include <string>
#include "student.hpp"

using namespace std;

// Hàm đọc dữ liệu sinh viên từ file
bool importDataFromFile(const string& filename, vector<Student>& students, bool& hasInvalidRows);

#endif