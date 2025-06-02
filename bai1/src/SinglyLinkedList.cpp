#include "SinglyLinkedList.hpp"
#include "console_utils.hpp"
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <fmt/core.h>
#include <fmt/format.h> 
#include <chrono>

// Constructor
SinglyLinkedList::SinglyLinkedList() : head(nullptr), size(0) {}

// Destructor
SinglyLinkedList::~SinglyLinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    size = 0;
}

// Chuyển dữ liệu từ vector data vào cấu trúc danh sách liên kết đơn
void SinglyLinkedList::transferFromVector(const std::vector<Student>& initialStudents) {
    // Xóa danh sách hiện tại nếu có
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    size = 0;
    
    // Thêm từng sinh viên vào danh sách
    for (const auto& student : initialStudents) {
        add(student);
    }
}

// Thêm sinh viên vào danh sách
bool SinglyLinkedList::add(const Student& student) {
    // Kiểm tra trùng lặp MSSV
    Node* current = head;
    while (current != nullptr) {
        if (current->data.mssv == student.mssv) {
            std::cerr << "MSSV đã tồn tại trong danh sách sinh viên.\n";
            return false;
        }
        current = current->next;
    }
    
    // Tạo node mới
    Node* newNode = new Node(student);
    
    // Nếu danh sách rỗng, thêm node mới làm head
    if (head == nullptr) {
        head = newNode;
    } else {
        // Duyệt đến cuối danh sách
        current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        // Thêm node mới vào cuối danh sách
        current->next = newNode;
    }
    
    size++;
    return true;
}

// Cập nhật thông tin sinh viên trong danh sách
bool SinglyLinkedList::update(const std::string& studentId, const Student& updatedStudent) {
    Node* current = head;
    
    while (current != nullptr) {
        if (current->data.mssv == studentId) {
            // Tạo sinh viên tạm để lưu thông tin cập nhật
            Student tempStudent = updatedStudent;
            
            // Giữ nguyên thông tin nếu không cập nhật
            if(tempStudent.mssv.empty()) {
                tempStudent.mssv = current->data.mssv;
            }
            if(tempStudent.ho.empty()) {
                tempStudent.ho = current->data.ho;
            }
            if(tempStudent.ten.empty()) {
                tempStudent.ten = current->data.ten;
            }
            if(tempStudent.lop.empty()) {
                tempStudent.lop = current->data.lop;
            }
            if(tempStudent.diem < 0.0f) {
                // If diem is negative (our special marker), don't update it
                tempStudent.diem = current->data.diem;
            }
            
            // Kiểm tra trùng lặp MSSV nếu đổi MSSV
            if (studentId != tempStudent.mssv) {
                Node* checkNode = head;
                while (checkNode != nullptr) {
                    if (checkNode != current && checkNode->data.mssv == tempStudent.mssv) {
                        std::cerr << "MSSV đã tồn tại trong danh sách sinh viên.\n";
                        return false;
                    }
                    checkNode = checkNode->next;
                }
            }
            
            // Cập nhật thông tin sinh viên
            current->data = tempStudent;
            return true;
        }
        current = current->next;
    }
    
    return false; // Không tìm thấy sinh viên với MSSV đã cung cấp
}

// Xóa sinh viên khỏi danh sách
bool SinglyLinkedList::remove(const std::string& studentId) {
    if (head == nullptr) {
        return false;
    }
    
    // Nếu xóa node đầu
    if (head->data.mssv == studentId) {
        Node* temp = head;
        head = head->next;
        delete temp;
        size--;
        return true;
    }
    
    // Tìm node cần xóa
    Node* current = head;
    while (current->next != nullptr && current->next->data.mssv != studentId) {
        current = current->next;
    }
    
    // Nếu tìm thấy
    if (current->next != nullptr) {
        Node* temp = current->next;
        current->next = temp->next;
        delete temp;
        size--;
        return true;
    }
    
    return false; // Không tìm thấy sinh viên với MSSV đã cung cấp
}

// Lấy danh sách sinh viên điểm thấp nhất
bool SinglyLinkedList::listLowestScoringStudents() const {
    if (head == nullptr) {
        return false; // Danh sách rỗng
    }
    
    // Tìm điểm thấp nhất
    float lowestScore = head->data.diem;
    Node* current = head->next;
    
    while (current != nullptr) {
        if (current->data.diem < lowestScore) {
            lowestScore = current->data.diem;
        }
        current = current->next;
    }
    
    // Hiển thị sinh viên có điểm thấp nhất
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
    current = head;
    
    while (current != nullptr) {
        if (current->data.diem == lowestScore) {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
                      stt++, current->data.mssv, current->data.ho, current->data.ten, current->data.lop, current->data.diem);
        }
        current = current->next;
    }
    
    return true;
}

// Lấy danh sách sinh viên điểm cao nhất
bool SinglyLinkedList::listHighestScoringStudents() const {
    if (head == nullptr) {
        return false; 
    }
    
    // Tìm điểm cao nhất
    float highestScore = head->data.diem;
    Node* current = head->next;
    
    while (current != nullptr) {
        if (current->data.diem > highestScore) {
            highestScore = current->data.diem;
        }
        current = current->next;
    }
    
    // Hiển thị sinh viên có điểm cao nhất
    std::cout << "Điểm thấp nhất: ";
    SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
    std::cout << highestScore << "\n";
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cout << "Danh sách sinh viên có điểm thấp nhất:\n\n";
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10}\n",
              "STT", "MSSV", "Họ", "Tên", "Lớp", "Điểm");
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cout << std::string(85, '-') << std::endl;
    
    int stt = 1;
    current = head;
    
    while (current != nullptr) {
        if (current->data.diem == highestScore) {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
                      stt++, current->data.mssv, current->data.ho, current->data.ten, current->data.lop, current->data.diem);
        }
        current = current->next;
    }
    
    return true;
}

// Tính điểm trung bình của từng lớp
double SinglyLinkedList::calculateAverageScore() const {
    if (head == nullptr) {
        std::cout << "Không có sinh viên nào trong danh sách." << std::endl;
        return 0.0;
    }
    
    // Map để lưu tổng điểm và số lượng sinh viên của từng lớp
    std::map<std::string, std::pair<double, int>> classStats;
    
    // Tính tổng điểm và đếm số sinh viên cho từng lớp
    Node* current = head;
    while (current != nullptr) {
        const std::string& className = current->data.lop;
        double score = current->data.diem;
        
        if (classStats.find(className) == classStats.end()) {
            // Lớp mới, khởi tạo với điểm đầu tiên và số lượng = 1
            classStats[className] = std::make_pair(score, 1);
        } else {
            // Lớp đã tồn tại, cập nhật tổng điểm và số lượng
            classStats[className].first += score;
            classStats[className].second++;
        }
        
        current = current->next;
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

// Lấy tất cả sinh viên trong danh sách
std::vector<Student> SinglyLinkedList::getAllStudents() const {
    std::vector<Student> allStudents;
    Node* current = head;
    
    while (current != nullptr) {
        allStudents.push_back(current->data);
        current = current->next;
    }
    
    return allStudents;
}

// Sort - Chưa triển khai
double SinglyLinkedList::sort(SortAlgorithmType algorithm, SortCriterionType criterion) {
    throw std::logic_error("Sort function is not implemented for SinglyLinkedList.");
}

// Search 
SearchResult SinglyLinkedList::search(SearchCriterionType criterion, const std::string& searchTerm, bool reverseName, int searchAlgoChoice) const {
    if (searchAlgoChoice == 1) {
        throw std::logic_error("There is no binary search in singly linked list");

    } else {
        using namespace std::chrono;
        std::vector<Student> resultOfSearch;
        auto start = high_resolution_clock::now();

        Node* current = head;

        if (current == nullptr) {
            auto end = high_resolution_clock::now();
            duration<double, std::milli> elapsed = end - start;
            SearchResult result = SearchResult(resultOfSearch, elapsed.count());
            return result;
        }

        while (current != nullptr) {
            const Student& student = current->data;
            bool isMatched = false;

            switch (criterion) {
                case SearchCriterionType::DIEM: {
                    std::string diem = std::to_string(student.diem);
                    isMatched = containsSubString(diem, searchTerm);
                    break;
                }
                case SearchCriterionType::HO:   isMatched = containsSubString(student.ho, searchTerm); break;
                case SearchCriterionType::TEN:  isMatched = containsSubString(student.ten, searchTerm); break;
                case SearchCriterionType::MSSV: isMatched = containsSubString(student.mssv, searchTerm); break;
                case SearchCriterionType::LOP:  isMatched = containsSubString(student.lop, searchTerm); break;
            }

            if (isMatched) {
                resultOfSearch.push_back(student);
            }

            current = current -> next;
        }

        auto end = high_resolution_clock::now();
        duration<double, std::milli> elapsed = end - start;
        SearchResult result = SearchResult(resultOfSearch, elapsed.count());
        return result;
    }
    
}