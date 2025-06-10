#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <algorithm>
#include <vector>
#include <set> // Required for mssvInCurrentFile in validateMSSV

using namespace std;

// trim khoảng trắng ở đầu và cuối chuỗi
string trim(const string& str);

// các hàm kiểm tra dữ liệu
bool validateMSSV(const string& mssv, const set<string>& mssvInCurrentFile, vector<string>& errorMessages);
bool validateHo(const string& ho, vector<string>& errorMessages);
bool validateTen(const string& ten, vector<string>& errorMessages);
bool validateLop(const string& lop, vector<string>& errorMessages);
bool validateDiem(const string& diemStr, float& diemOut, vector<string>& errorMessages);

// Chuyển về chữ thường 
string toLowerString(const string& s);

// Chuyển về chữ hoa
string toUpperString(const string& s);

// Chuẩn hóa tên (viết hoa chữ cái đầu mỗi từ)
string capitalizeName(const string& name);

// Kiểm tra Xâu có chứa nội dung cần tìm không 
bool containsSubString(const string& text, const string& searchTerms);

// Kiểm tra xem prefix có là tiền tố của text không
bool startsWith(const std::string& text, const std::string& prefix);


#endif
