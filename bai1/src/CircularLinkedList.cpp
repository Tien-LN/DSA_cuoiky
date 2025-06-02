#include "CircularLinkedList.hpp"
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
CircularLinkedList::CircularLinkedList() : head(nullptr), tail(nullptr), size(0) {}

// Destructor
CircularLinkedList::~CircularLinkedList() {
    if (head == nullptr) return;
    
    // Ngắt vòng tròn trước khi xóa
    if (tail != nullptr) {
        tail->next = nullptr;
    }
    
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Chuyển dữ liệu từ vector data vào cấu trúc danh sách liên kết vòng
void CircularLinkedList::transferFromVector(const std::vector<Student>& initialStudents) {
    // Xóa danh sách hiện tại nếu có
    if (head != nullptr) {
        // Ngắt vòng tròn trước khi xóa
        if (tail != nullptr) {
            tail->next = nullptr;
        }
        
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }
    
    // Thêm từng sinh viên vào danh sách
    for (const auto& student : initialStudents) {
        add(student);
    }
}

// Thêm sinh viên vào danh sách
bool CircularLinkedList::add(const Student& student) {
    // Kiểm tra trùng lặp MSSV
    if (head != nullptr) {
        Node* current = head;
        do {
            if (current->data.mssv == student.mssv) {
                std::cerr << "MSSV đã tồn tại trong danh sách sinh viên.\n";
                return false;
            }
            current = current->next;
        } while (current != head);
    }
    
    // Tạo node mới
    Node* newNode = new Node(student);
    
    // Nếu danh sách rỗng
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
        // Tạo vòng tròn
        newNode->next = newNode;
    } else {
        // Thêm vào cuối danh sách
        newNode->next = head;
        tail->next = newNode;
        tail = newNode;
    }
    
    size++;
    return true;
}

// Cập nhật thông tin sinh viên trong danh sách
bool CircularLinkedList::update(const std::string& studentId, const Student& updatedStudent) {
    if (head == nullptr) {
        return false; // Danh sách rỗng
    }
    
    Node* current = head;
    
    do {
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
                tempStudent.diem = current->data.diem;
            }
            
            // Kiểm tra trùng lặp MSSV nếu đổi MSSV
            if (studentId != tempStudent.mssv) {
                Node* checkNode = head;
                do {
                    if (checkNode != current && checkNode->data.mssv == tempStudent.mssv) {
                        std::cerr << "MSSV đã tồn tại trong danh sách sinh viên.\n";
                        return false;
                    }
                    checkNode = checkNode->next;
                } while (checkNode != head);
            }
            
            // Cập nhật thông tin sinh viên
            current->data = tempStudent;
            return true;
        }
        current = current->next;
    } while (current != head);
    
    return false; // Không tìm thấy sinh viên với MSSV đã cung cấp
}

// Xóa sinh viên khỏi danh sách
bool CircularLinkedList::remove(const std::string& studentId) {
    if (head == nullptr) {
        return false; 
    }
    
    // Nếu chỉ có một node
    if (head == tail) {
        if (head->data.mssv == studentId) {
            delete head;
            head = nullptr;
            tail = nullptr;
            size = 0;
            return true;
        }
        return false;
    }
    
    // Nếu xóa node đầu tiên
    if (head->data.mssv == studentId) {
        Node* temp = head;
        head = head->next;
        tail->next = head;
        delete temp;
        size--;
        return true;
    }
    
    // Tìm node trước node cần xóa
    Node* current = head;
    while (current->next != head && current->next->data.mssv != studentId) {
        current = current->next;
    }
    
    // Nếu tìm thấy
    if (current->next != head) {
        Node* temp = current->next;
        
        // Nếu là node cuối
        if (temp == tail) {
            tail = current;
        }
        
        current->next = temp->next;
        delete temp;
        size--;
        return true;
    }
    
    return false; // Không tìm thấy sinh viên với MSSV đã cung cấp
}

// Lấy danh sách sinh viên điểm thấp nhất
bool CircularLinkedList::listLowestScoringStudents() const {
    if (head == nullptr) {
        return false; // Danh sách rỗng
    }
    
    // Tìm điểm thấp nhất
    float lowestScore = head->data.diem;
    Node* current = head->next;
    
    while (current != head) {
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
    
    do {
        if (current->data.diem == lowestScore) {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
                      stt++, current->data.mssv, current->data.ho, current->data.ten, current->data.lop, current->data.diem);
        }
        current = current->next;
    } while (current != head);
    
    return true;
}

// Lấy danh sách sinh viên điểm cao nhất
bool CircularLinkedList::listHighestScoringStudents() const {
    if (head == nullptr) {
        return false; // Danh sách rỗng
    }
    
    // Tìm điểm cao nhất
    float highestScore = head->data.diem;
    Node* current = head->next;
    
    while (current != head) {
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
    
    do {
        if (current->data.diem == highestScore) {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
                      stt++, current->data.mssv, current->data.ho, current->data.ten, current->data.lop, current->data.diem);
        }
        current = current->next;
    } while (current != head);
    
    return true;
}

// Tính điểm trung bình của từng lớp
double CircularLinkedList::calculateAverageScore() const {
    if (head == nullptr) {
        std::cout << "Không có sinh viên nào trong danh sách." << std::endl;
        return 0.0;
    }
    
    // Map để lưu tổng điểm và số lượng sinh viên của từng lớp
    std::map<std::string, std::pair<double, int>> classStats;
    
    // Tính tổng điểm và đếm số sinh viên cho từng lớp
    Node* current = head;
    
    do {
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
    } while (current != head);
    
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
std::vector<Student> CircularLinkedList::getAllStudents() const {
    std::vector<Student> allStudents;
    
    if (head == nullptr) {
        return allStudents;
    }
    
    Node* current = head;
    
    do {
        allStudents.push_back(current->data);
        current = current->next;
    } while (current != head);
    
    return allStudents;
}

// Sort - Chưa triển khai
double CircularLinkedList::sort(SortAlgorithmType algorithm, SortCriterionType criterion) {
    throw std::logic_error("Sort function is not implemented for CircularLinkedList.");
}

// Search 
SearchResult CircularLinkedList::search(SearchCriterionType criterion, const std::string& searchTerm, bool reverseName, int searchAlgoChoice) const {
    if(searchAlgoChoice == 1) {
        throw std::logic_error("There is not binary search in Circular Linked List");
    } else {
        std::vector<Student> resultOfSearch;
        using namespace std::chrono;
        auto start = high_resolution_clock::now();

        // There is empty list 
        if (head == nullptr) {
            auto end = high_resolution_clock::now();
            duration<double, std::milli> elapsed = end - start;
            SearchResult result = SearchResult(resultOfSearch, elapsed.count());
            return result;
        }

        Node* current = head;

        do {
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
        } while (current != head);

        auto end = high_resolution_clock::now();
        duration<double, std::milli> elapsed = end - start;
        SearchResult result = SearchResult(resultOfSearch, elapsed.count());
        return result;
    }
}