#include "utils.hpp"
#include <vector>
#include <set>
#include <stdexcept>

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
        if (mssv.find(' ') != string::npos || mssv.find('\t') != string::npos) {
            isValid = false;
            errorMessages.push_back("Mã sinh viên không được chứa khoảng trắng.");
        }
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
    if (lop.empty()) {
        errorMessages.push_back("Lớp không được để trống.");
        isValid = false;
    } else if (lop.find(' ') != string::npos || lop.find('\t') != string::npos) {
        errorMessages.push_back("Lớp không được chứa khoảng trắng.");
        isValid = false;
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