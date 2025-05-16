#include "DoublyLinkedList.hpp"
#include "console_utils.hpp"
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <fmt/core.h>
#include <fmt/format.h> 

// Constructor
DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

// Destructor
DoublyLinkedList::~DoublyLinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
        if (current == head) break; // In case we have a circular reference
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Chuyển dữ liệu từ vector data vào cấu trúc danh sách liên kết
void DoublyLinkedList::transferFromVector(const std::vector<Student>& initialStudents) {
    // Xóa danh sách hiện tại nếu có
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
        if (current == head) break; // In case we have a circular reference
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
    
    // Thêm từng sinh viên vào danh sách
    for (const auto& student : initialStudents) {
        add(student);
    }
}

// Thêm sinh viên vào danh sách
bool DoublyLinkedList::add(const Student& student) {
    // Kiểm tra trùng lặp MSSV
    Node* current = head;
    for (int i = 0; i < size; i++) {
        if (current->data.mssv == student.mssv) {
            std::cerr << "MSSV đã tồn tại trong danh sách sinh viên.\n";
            return false;
        }
        current = current->next;
    }
    
    // Tạo node mới
    Node* newNode = new Node(student);
    
    // Nếu danh sách rỗng
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        // Thêm vào cuối danh sách
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    
    size++;
    return true;
}

// Cập nhật thông tin sinh viên trong danh sách
bool DoublyLinkedList::update(const std::string& studentId, const Student& updatedStudent) {
    Node* current = head;
    
    for (int i = 0; i < size; i++) {
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
                for (int j = 0; j < size; j++) {
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
bool DoublyLinkedList::remove(const std::string& studentId) {
    if (head == nullptr) {
        return false; // Danh sách rỗng
    }
    
    Node* current = head;
    
    for (int i = 0; i < size; i++) {
        if (current->data.mssv == studentId) {
            // Nếu chỉ có một node
            if (size == 1) {
                delete current;
                head = nullptr;
                tail = nullptr;
            }
            // Nếu là node đầu tiên
            else if (current == head) {
                head = head->next;
                head->prev = nullptr;
                delete current;
            }
            // Nếu là node cuối cùng
            else if (current == tail) {
                tail = tail->prev;
                tail->next = nullptr;
                delete current;
            }
            // Nếu là node ở giữa
            else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                delete current;
            }
            
            size--;
            return true;
        }
        current = current->next;
    }
    
    return false; // Không tìm thấy sinh viên với MSSV đã cung cấp
}

// Lấy danh sách sinh viên điểm thấp nhất
bool DoublyLinkedList::listLowestScoringStudents() const {
    if (head == nullptr) {
        return false; // Danh sách rỗng
    }
    
    // Tìm điểm thấp nhất
    float lowestScore = head->data.diem;
    Node* current = head->next;
    
    for (int i = 1; i < size; i++) {
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
    
    for (int i = 0; i < size; i++) {
        if (current->data.diem == lowestScore) {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
                      stt++, current->data.mssv, current->data.ho, current->data.ten, current->data.lop, current->data.diem);
        }
        current = current->next;
    }
    
    return true;
}

// Lấy danh sách sinh viên điểm cao nhất
bool DoublyLinkedList::listHighestScoringStudents() const {
    if (head == nullptr) {
        return false; // Danh sách rỗng
    }
    
    // Tìm điểm cao nhất
    float highestScore = head->data.diem;
    Node* current = head->next;
    
    for (int i = 1; i < size; i++) {
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
    
    for (int i = 0; i < size; i++) {
        if (current->data.diem == highestScore) {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
                      stt++, current->data.mssv, current->data.ho, current->data.ten, current->data.lop, current->data.diem);
        }
        current = current->next;
    }
    
    return true;
}

// Tính điểm trung bình của từng lớp
double DoublyLinkedList::calculateAverageScore() const {
    if (head == nullptr) {
        std::cout << "Không có sinh viên nào trong danh sách." << std::endl;
        return 0.0;
    }
    
    // Map để lưu tổng điểm và số lượng sinh viên của từng lớp
    std::map<std::string, std::pair<double, int>> classStats;
    
    // Tính tổng điểm và đếm số sinh viên cho từng lớp
    Node* current = head;
    for (int i = 0; i < size; i++) {
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
std::vector<Student> DoublyLinkedList::getAllStudents() const {
    std::vector<Student> allStudents;
    Node* current = head;
    
    for (int i = 0; i < size; i++) {
        allStudents.push_back(current->data);
        current = current->next;
    }
    
    return allStudents;
}

// Sort - Chưa triển khai
double DoublyLinkedList::sort(SortAlgorithmType algorithm, SortCriterionType criterion) {
    throw std::logic_error("Sort function is not implemented for DoublyLinkedList.");
}

// Search - Chưa triển khai
SearchResult DoublyLinkedList::search(SearchCriterionType criterion, const std::string& searchTerm, bool reverseName) const {
    throw std::logic_error("Search function is not implemented for DoublyLinkedList.");
}