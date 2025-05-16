#include "Array.hpp"
#include "console_utils.hpp"
#include <stdexcept> 
#include <algorithm> 
#include <vector>
#include <iostream> 
#include <iomanip>
#include <map>
#include <fmt/core.h>
#include <fmt/format.h> 

// Constructor
StudentArray::StudentArray() : currentSize(0) {}

// Chuyển dữ liệu từ vector data vào cấu trúc mảng
void StudentArray::transferFromVector(const std::vector<Student>& initialStudents) {
    if (initialStudents.size() > MAX_STUDENTS) {
        throw std::out_of_range("Số lượng sinh viên vượt quá giới hạn cho phép.");
    }

    currentSize = 0;
    for (const auto& student : initialStudents) {
        if (currentSize < MAX_STUDENTS) {
            students[currentSize++] = student;
        } else {
            break;
        }
    }
}

// thêm sinh viên vào mảng
bool StudentArray::add(const Student& student) {
    if (currentSize >= MAX_STUDENTS) {
        throw std::out_of_range("Không thể thêm sinh viên: mảng đã đầy.");
    }
    // kiểm tra trùng lặp MSSV
    for (int i = 0; i < currentSize; ++i) {
        if (students[i].mssv == student.mssv) {
            std::cerr << "MSSV đã tồn tại trong danh sách sinh viên.\n";
            return false; 
        }
    }
    students[currentSize++] = student;
    return true;
}

// Cập nhật thông tin sinh viên trong mảng
bool StudentArray::update(const std::string& studentId, const Student& updatedStudent) {
    for (int i = 0; i < currentSize; ++i) {
        if (students[i].mssv == studentId) {
            
            Student tempStudent = updatedStudent;
            
            if(tempStudent.mssv.empty()) {
                tempStudent.mssv = students[i].mssv;
            }
            if(tempStudent.ho.empty()) {
                tempStudent.ho = students[i].ho;
            }
            if(tempStudent.ten.empty()) {
                tempStudent.ten = students[i].ten;
            }
            if(tempStudent.lop.empty()) {
                tempStudent.lop = students[i].lop;
            }
            if(tempStudent.diem < 0.0f) {
                // If diem is negative (our special marker), don't update it
                tempStudent.diem = students[i].diem;
            }
            
            // nếu người dùng cập nhật cả mssv thì kiểm tra xem có trùng lặp với sinh viên khác không
            if (studentId != tempStudent.mssv) {
                for (int j = 0; j < currentSize; ++j) {
                    if (i != j && students[j].mssv == tempStudent.mssv) {
                        std::cerr << "MSSV đã tồn tại trong danh sách sinh viên.\n";
                        return false; 
                    }
                }
            }
            students[i] = tempStudent;
            return true;
        }
    }
    return false; 
}

// Xóa sinh viên khỏi mảng
bool StudentArray::remove(const std::string& studentId) {
    for (int i = 0; i < currentSize; ++i) {
        if (students[i].mssv == studentId) {
            for (int j = i; j < currentSize - 1; ++j) {
                students[j] = students[j + 1];
            }
            currentSize--;
            students[currentSize] = Student(); // clear phần tử cuối cùng
            return true;
        }
    }
    return false; 
}

// lấy danh sách sinh viên điểm thấp nhất
bool StudentArray::listLowestScoringStudents() const {
    if (currentSize == 0) {
        return false;
    }
    
    float lowestScore = students[0].diem;
    for (int i = 1; i < currentSize; i++) {
        if (students[i].diem < lowestScore) {
            lowestScore = students[i].diem;
        }
    }
    
    std::cout << "Điểm thấp nhất: ";
    SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
    std::cout << lowestScore << "\n";
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cout << "Danh sách sinh viên có điểm thấp nhất:\n\n";
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10}\n",
              "STT", "MSSV", "Họ", "Tên", "Lớp", "Điểm");
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cout << std::string(85, '-') << std::endl;
    
    int stt = 1;
    for (int i = 0; i < currentSize; i++) {
        if (students[i].diem == lowestScore) {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
            i + 1, students[i].mssv, students[i].ho, students[i].ten, students[i].lop, students[i].diem);
        }
    }
    
    return true;
}

// lấy danh sách sinh viên điểm cao nhất
bool StudentArray::listHighestScoringStudents() const {
    if (currentSize == 0) {
        return false;
    }
    
    float highestScore = students[0].diem;
    for (int i = 1; i < currentSize; i++) {
        if (students[i].diem > highestScore) {
            highestScore = students[i].diem;
        }
    }
    
    std::cout << "Điểm cao nhất: ";
    SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
    std::cout << highestScore << "\n";
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cout << "Danh sách sinh viên có điểm cao nhất:\n\n";
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10}\n",
              "STT", "MSSV", "Họ", "Tên", "Lớp", "Điểm");
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cout << std::string(85, '-') << std::endl;
    
    int stt = 1;
    for (int i = 0; i < currentSize; i++) {
        if (students[i].diem == highestScore) {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
            i + 1, students[i].mssv, students[i].ho, students[i].ten, students[i].lop, students[i].diem);
        }
    }
    
    return true;
}

// lấy điểm trung bình của lớp
double StudentArray::calculateAverageScore() const {
    if (currentSize == 0) {
        std::cout << "Không có sinh viên nào trong danh sách." << std::endl;
        return 0.0;
    }
    
    // Map để lưu tổng điểm và số lượng sinh viên của từng lớp
    std::map<std::string, std::pair<double, int>> classStats;
    
    // Tính tổng điểm và đếm số sinh viên cho từng lớp
    for (int i = 0; i < currentSize; i++) {
        const std::string& className = students[i].lop;
        double score = students[i].diem;
        
        if (classStats.find(className) == classStats.end()) {
            // Lớp mới, khởi tạo với điểm đầu tiên và số lượng = 1
            classStats[className] = std::make_pair(score, 1);
        } else {
            // Lớp đã tồn tại, cập nhật tổng điểm và số lượng
            classStats[className].first += score;
            classStats[className].second++;
        }
    }
    
    // Hiển thị điểm trung bình của từng lớp
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    fmt::print("{:<20} {:<15} {:<20}\n", "Lớp", "Số sinh viên", "Điểm trung bình");
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cout << std::string(55, '-') << std::endl;
    
    double totalAverage = 0.0;
    int totalStudents = 0;
    
    for (const auto& entry : classStats) {
        const std::string& className = entry.first;
        double sum = entry.second.first;
        int count = entry.second.second;
        double average = sum / count;

        fmt::print("{:<20} {:<15} {:<20.2f}\n", className, count, average);
        
        totalAverage += sum;
        totalStudents += count;
    }
    
    // Hiển thị điểm trung bình tổng thể
    std::cout << std::string(55, '-') << std::endl;
    double overallAverage = totalAverage / totalStudents;
    SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
    fmt::print("{:<20} {:<15} {:<20.2f}\n", "Tổng cộng", totalStudents, overallAverage);
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    return overallAverage;
}

// lấy tất cả sinh viên trong mảng
std::vector<Student> StudentArray::getAllStudents() const {
    std::vector<Student> allStudentsVector;
    allStudentsVector.reserve(currentSize); 
    for (int i = 0; i < currentSize; ++i) {
        allStudentsVector.push_back(students[i]);
    }
    return allStudentsVector;
}

// Sort 
double StudentArray::sort(SortAlgorithmType algorithm, SortCriterionType criterion) {
    throw std::logic_error("Sort function is not implemented for StudentArray.");
}

// Search 
SearchResult StudentArray::search(SearchCriterionType criterion, const std::string& searchTerm, bool reverseName) const {
    throw std::logic_error("Search function is not implemented for StudentArray.");
}