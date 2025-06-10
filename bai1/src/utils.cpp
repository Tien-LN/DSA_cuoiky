#include "utils.hpp"
#include <vector>
#include <set>
#include <stdexcept>
#include <map>

using namespace std;

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n");
    size_t last = str.find_last_not_of(" \t\n");
    return (first == string::npos) ? "" : str.substr(first, last - first + 1);
}

bool equalsIgnoreCase(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (tolower(a[i]) != tolower(b[i])) return false;
    }
    return true;
}

bool validateMSSV(const string& mssv, const set<string>& mssvList, vector<string>& errorMessages) {
    bool isValid = true;
    
    if (mssv.empty()) {
        isValid = false;
        errorMessages.push_back("Mã sinh viên không được để trống.");
    } else {
        // Kiểm tra khoảng trắng
        if (mssv.find(' ') != string::npos || mssv.find('\t') != string::npos) {
            isValid = false;
            errorMessages.push_back("Mã sinh viên không được chứa khoảng trắng.");
        }
        
        // Kiểm tra ký tự đặc biệt
        bool hasSpecialChar = false;
        for (char c : mssv) {
            if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))) {
                hasSpecialChar = true;
                break;
            }
        }
        if (hasSpecialChar) {
            isValid = false;
            errorMessages.push_back("Mã sinh viên không được chứa ký tự đặc biệt.");
        }
        
        // Kiểm tra format NnnDCxxnnn (ví dụ: N20DCPT001)
        if (mssv.length() != 10) {
            isValid = false;
            errorMessages.push_back("Mã sinh viên phải có đúng 10 ký tự theo format NnnDCxxnnn.");
        } else {
            // Kiểm tra ký tự đầu phải là 'N'
            if (mssv[0] != 'N' && mssv[0] != 'n') {
                isValid = false;
                errorMessages.push_back("Mã sinh viên phải bắt đầu bằng ký tự 'N'.");
            }
            
            // Kiểm tra 2 ký tự tiếp theo phải là số (vị trí 1-2)
            if (!(mssv[1] >= '0' && mssv[1] <= '9') || !(mssv[2] >= '0' && mssv[2] <= '9')) {
                isValid = false;
                errorMessages.push_back("Vị trí thứ 2 và 3 của mã sinh viên phải là số.");
            }
            
            // Kiểm tra "DC" ở vị trí 3-4
            if ((mssv[3] != 'D' && mssv[3] != 'd') || (mssv[4] != 'C' && mssv[4] != 'c')) {
                isValid = false;
                errorMessages.push_back("Mã sinh viên phải chứa 'DC' ở vị trí thứ 4-5.");
            }
            
            // Kiểm tra 2 ký tự tiếp theo phải là chữ cái (vị trí 5-6)
            if (!((mssv[5] >= 'A' && mssv[5] <= 'Z') || (mssv[5] >= 'a' && mssv[5] <= 'z')) ||
                !((mssv[6] >= 'A' && mssv[6] <= 'Z') || (mssv[6] >= 'a' && mssv[6] <= 'z'))) {
                isValid = false;
                errorMessages.push_back("Vị trí thứ 6 và 7 của mã sinh viên phải là chữ cái.");
            }
            
            // Kiểm tra 3 ký tự cuối phải là số (vị trí 7-9)
            if (!(mssv[7] >= '0' && mssv[7] <= '9') || 
                !(mssv[8] >= '0' && mssv[8] <= '9') || 
                !(mssv[9] >= '0' && mssv[9] <= '9')) {
                isValid = false;
                errorMessages.push_back("3 ký tự cuối của mã sinh viên phải là số.");
            }
        }
        
        // Kiểm tra trùng lặp
        if (mssvList.count(mssv)) {
            isValid = false;
            errorMessages.push_back("Mã sinh viên bị trùng với một sinh viên trước trong file.");
        }
    }
    return isValid;
}

bool validateHo(const string& ho, vector<string>& errorMessages) {
    if (ho.empty()) {
        errorMessages.push_back("Họ không được để trống.");
        return false;
    }
    return true;
}

bool validateTen(const string& ten, vector<string>& errorMessages) {
    if (ten.empty()) {
        errorMessages.push_back("Tên không được để trống.");
        return false;
    }
    return true;
}

bool validateLop(const string& lop, vector<string>& errorMessages) {
    bool isValid = true; 

    // Rule 1: Không được để trống
    if (lop.empty()) {
        errorMessages.push_back("Lớp không được để trống.");
        return false; 
    }

    // Rule 2: Không được chứa khoảng trắng (dấu cách hoặc tab)
    for (size_t i = 0; i < lop.length(); ++i) {
        if (lop[i] == ' ' || lop[i] == '\t') {
            errorMessages.push_back("Mã Lớp không được chứa khoảng trắng.");
            return false; 
        }
    }

    // Chuyển 'lop' về chữ hoa để kiểm tra không phân biệt chữ hoa/thường cho các ký tự chữ
    string upperLop = "";
    for (size_t i = 0; i < lop.length(); ++i) {
        char c = lop[i];
        if (c >= 'a' && c <= 'z') {
            upperLop += (char)(c - ('a' - 'A')); // Chuyển thành chữ hoa
        } else {
            upperLop += c;
        }
    }

    // Rule 3: Kiểm tra format DnnCQxxnn (ví dụ: D21CQCN01) - độ dài 9
    if (upperLop.length() != 9) {
        errorMessages.push_back("Lớp phải có đúng 9 ký tự theo format DnnCQxxnn.");
        isValid = false;
    } else {
        // Kiểm tra ký tự đặc biệt
        bool hasSpecialChar = false;
        for (char c : lop) {
            if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))) {
                hasSpecialChar = true;
                break;
            }
        }
        if (hasSpecialChar) {
            isValid = false;
            errorMessages.push_back("Mã lớp không được chứa ký tự đặc biệt.");
        }

        // Kiểm tra ký tự đầu phải là 'D'
        if (upperLop[0] != 'D') {
            errorMessages.push_back("Lớp phải bắt đầu bằng ký tự 'D'.");
            isValid = false;
        }
        
        // Kiểm tra 2 ký tự tiếp theo phải là số (vị trí 1-2)
        if (!((upperLop[1] >= '0' && upperLop[1] <= '9') && (upperLop[2] >= '0' && upperLop[2] <= '9'))) {
            errorMessages.push_back("Vị trí thứ 2 và 3 của lớp phải là số.");
            isValid = false;
        }
        
        // Kiểm tra "CQ" ở vị trí 3-4
        if (upperLop[3] != 'C' || upperLop[4] != 'Q') {
            errorMessages.push_back("Lớp phải chứa 'CQ' ở vị trí thứ 4-5.");
            isValid = false;
        }
        
        // Kiểm tra 2 ký tự tiếp theo phải là chữ cái (vị trí 5-6)
        if (!(((upperLop[5] >= 'A' && upperLop[5] <= 'Z')) && ((upperLop[6] >= 'A' && upperLop[6] <= 'Z')))) {
            errorMessages.push_back("Vị trí thứ 6 và 7 của lớp phải là chữ cái.");
            isValid = false;
        }
        
        // Kiểm tra 2 ký tự cuối phải là số (vị trí 7-8)
        if (!((upperLop[7] >= '0' && upperLop[7] <= '9') && (upperLop[8] >= '0' && upperLop[8] <= '9'))) {
            errorMessages.push_back("2 ký tự cuối của lớp phải là số.");
            isValid = false;
        }
    }
    return isValid;
}

bool validateDiem(const string& diemStr, float& diemOut, vector<string>& errorMessages) {
    bool isValid = true;
    if (diemStr.empty()) {
        errorMessages.push_back("Điểm không được để trống.");
        isValid = false;
    } else {
        try {
            size_t processedChars = 0;
            diemOut = stof(diemStr, &processedChars);
            if (processedChars != diemStr.length()) {
                errorMessages.push_back("Điểm chứa ký tự không hợp lệ.");
                isValid = false;
            } else if (diemOut < 0.0f || diemOut > 10.0f) {
                errorMessages.push_back("Điểm không hợp lệ (phải trong khoảng từ 0.0 đến 10.0).");
                isValid = false;
            }        } catch (const invalid_argument&) {
            errorMessages.push_back("Định dạng điểm không hợp lệ (không phải là số).");
            isValid = false;
        } catch (const out_of_range&) {
            errorMessages.push_back("Điểm nằm ngoài phạm vi giá trị có thể biểu diễn.");
            isValid = false;
        }
    }
    return isValid;
}

// Chuyển về chữ thường 
string toLowerString(const string& s){ 
    string result = s;
    for (char& c : result){
        c = (c >= 'A' && c <= 'Z') ? (char)(c + 32) : c;
    }
    return result;
}

// Chuyển về chữ hoa
string toUpperString(const string& s) {
    string result = s;
    for (char& c : result) {
        c = (c >= 'a' && c <= 'z') ? (char)(c - 32) : c;
    }
    return result;
}

// Hàm hỗ trợ chuyển đổi ký tự tiếng Việt thành chữ hoa
string convertVietnameseToUpper(const string& utf8Char) {
    // Bảng ánh xạ các ký tự tiếng Việt thường -> hoa
    static const map<string, string> lowerToUpper = {
        // Ă
        {"ă", "Ă"}, {"ắ", "Ắ"}, {"ằ", "Ằ"}, {"ẳ", "Ẳ"}, {"ẵ", "Ẵ"}, {"ặ", "Ặ"},
        // Â
        {"â", "Â"}, {"ấ", "Ấ"}, {"ầ", "Ầ"}, {"ẩ", "Ẩ"}, {"ẫ", "Ẫ"}, {"ậ", "Ậ"},
        // Đ
        {"đ", "Đ"},
        // Ê
        {"ê", "Ê"}, {"ế", "Ế"}, {"ề", "Ề"}, {"ể", "Ể"}, {"ễ", "Ễ"}, {"ệ", "Ệ"},
        // Ô
        {"ô", "Ô"}, {"ố", "Ố"}, {"ồ", "Ồ"}, {"ổ", "Ổ"}, {"ỗ", "Ỗ"}, {"ộ", "Ộ"},
        // Ơ
        {"ơ", "Ơ"}, {"ớ", "Ớ"}, {"ờ", "Ờ"}, {"ở", "Ở"}, {"ỡ", "Ỡ"}, {"ợ", "Ợ"},
        // Ư
        {"ư", "Ư"}, {"ứ", "Ứ"}, {"ừ", "Ừ"}, {"ử", "Ử"}, {"ữ", "Ữ"}, {"ự", "Ự"},
        // Y
        {"ý", "Ý"}, {"ỳ", "Ỳ"}, {"ỷ", "Ỷ"}, {"ỹ", "Ỹ"}, {"ỵ", "Ỵ"},
        // Các nguyên âm có dấu khác
        {"á", "Á"}, {"à", "À"}, {"ả", "Ả"}, {"ã", "Ã"}, {"ạ", "Ạ"},
        {"é", "É"}, {"è", "È"}, {"ẻ", "Ẻ"}, {"ẽ", "Ẽ"}, {"ẹ", "Ẹ"},
        {"í", "Í"}, {"ì", "Ì"}, {"ỉ", "Ỉ"}, {"ĩ", "Ĩ"}, {"ị", "Ị"},
        {"ó", "Ó"}, {"ò", "Ò"}, {"ỏ", "Ỏ"}, {"õ", "Õ"}, {"ọ", "Ọ"},
        {"ú", "Ú"}, {"ù", "Ù"}, {"ủ", "Ủ"}, {"ũ", "Ũ"}, {"ụ", "Ụ"}
    };
    
    auto it = lowerToUpper.find(utf8Char);
    return (it != lowerToUpper.end()) ? it->second : utf8Char;
}

// Hàm hỗ trợ chuyển đổi ký tự tiếng Việt thành chữ thường
string convertVietnameseToLower(const string& utf8Char) {
    // Bảng ánh xạ các ký tự tiếng Việt hoa -> thường
    static const map<string, string> upperToLower = {
        // Ă
        {"Ă", "ă"}, {"Ắ", "ắ"}, {"Ằ", "ằ"}, {"Ẳ", "ẳ"}, {"Ẵ", "ẵ"}, {"Ặ", "ặ"},
        // Â
        {"Â", "â"}, {"Ấ", "ấ"}, {"Ầ", "ầ"}, {"Ẩ", "ẩ"}, {"Ẫ", "ẫ"}, {"Ậ", "ậ"},
        // Đ
        {"Đ", "đ"},
        // Ê
        {"Ê", "ê"}, {"Ế", "ế"}, {"Ề", "ề"}, {"Ể", "ể"}, {"Ễ", "ễ"}, {"Ệ", "ệ"},
        // Ô
        {"Ô", "ô"}, {"Ố", "ố"}, {"Ồ", "ồ"}, {"Ổ", "ổ"}, {"Ỗ", "ỗ"}, {"Ộ", "ộ"},
        // Ơ
        {"Ơ", "ơ"}, {"Ớ", "ớ"}, {"Ờ", "ờ"}, {"Ở", "ở"}, {"Ỡ", "ỡ"}, {"Ợ", "ợ"},
        // Ư
        {"Ư", "ư"}, {"Ứ", "ứ"}, {"Ừ", "ừ"}, {"Ử", "ử"}, {"Ữ", "ữ"}, {"Ự", "ự"},
        // Y
        {"Ý", "ý"}, {"Ỳ", "ỳ"}, {"Ỷ", "ỷ"}, {"Ỹ", "ỹ"}, {"Ỵ", "ỵ"},
        // Các nguyên âm có dấu khác
        {"Á", "á"}, {"À", "à"}, {"Ả", "ả"}, {"Ã", "ã"}, {"Ạ", "ạ"},
        {"É", "é"}, {"È", "è"}, {"Ẻ", "ẻ"}, {"Ẽ", "ẽ"}, {"Ẹ", "ẹ"},
        {"Í", "í"}, {"Ì", "ì"}, {"Ỉ", "ỉ"}, {"Ĩ", "ĩ"}, {"Ị", "ị"},
        {"Ó", "ó"}, {"Ò", "ò"}, {"Ỏ", "ỏ"}, {"Õ", "õ"}, {"Ọ", "ọ"},
        {"Ú", "ú"}, {"Ù", "ù"}, {"Ủ", "ủ"}, {"Ũ", "ũ"}, {"Ụ", "ụ"}
    };
    
    auto it = upperToLower.find(utf8Char);
    return (it != upperToLower.end()) ? it->second : utf8Char;
}

// Chuẩn hóa tên (viết hoa chữ cái đầu mỗi từ)
string capitalizeName(const string& name) {
    if (name.empty()) return name;
    
    string result;
    bool capitalizeNext = true;
    
    for (size_t i = 0; i < name.length(); ) {
        unsigned char c = name[i];
        
        // Ký tự ASCII
        if (c < 128) {
            if (c == ' ' || c == '\t') {
                result += c;
                capitalizeNext = true;
                i++;
            } else if (capitalizeNext && c >= 'a' && c <= 'z') {
                result += (c - 32);
                capitalizeNext = false; 
                i++;
            } else if (!capitalizeNext && c >= 'A' && c <= 'Z') {
                result += (c + 32);
                i++;
            } else {
                result += c;
                capitalizeNext = false;
                i++;
            }
        }
        // Ký tự UTF-8 (2-4 bytes)
        else {
            // Xác định số byte của ký tự UTF-8
            int byteCount = 1;
            if ((c & 0xE0) == 0xC0) byteCount = 2;
            else if ((c & 0xF0) == 0xE0) byteCount = 3;
            else if ((c & 0xF8) == 0xF0) byteCount = 4;
            
            string utf8Char = name.substr(i, byteCount);
            
            if (capitalizeNext) {
                // Chuyển đổi ký tự tiếng Việt thường thành hoa
                utf8Char = convertVietnameseToUpper(utf8Char);
                capitalizeNext = false;
            } else {
                // Chuyển đổi ký tự tiếng Việt hoa thành thường
                utf8Char = convertVietnameseToLower(utf8Char);
            }
            
            result += utf8Char;
            i += byteCount;
        }
    }
    
    return result;
}


// hàm kiểm tra xâu có chứa nội dung tìm hay không 
bool containsSubString(const string& text, const string& searchTerms){
    size_t n = text.size();
    size_t m = searchTerms.size();

    if (m == 0) return true;
    if (m > n) return false;

    string c1 = toLowerString(text);
    string c2 = toLowerString(searchTerms);

    for (size_t i = 0; i<= n - m; i++){
        bool found = true;
        for (int j = 0; j < m; j++) {
            if(c1[i + j] != c2[j]){
                found = false;
                break;
            }
        }
        if (found) return true;
    }
    return false;
}


// Hàm Kiểm tra có là tiền tố của text không
bool startsWith(const std::string& text, const std::string& prefix) {
    if (prefix.size() > text.size()) return false;
    for (size_t i = 0; i < prefix.size(); ++i) {
        if (tolower(text[i]) != tolower(prefix[i])) {
            return false;
        }
    }
    return true;
}

