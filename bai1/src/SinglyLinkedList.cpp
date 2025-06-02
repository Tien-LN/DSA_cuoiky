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
#include <functional>

// Constructor
SinglyLinkedList::SinglyLinkedList() : head(nullptr), size(0) {}

// Destructor
SinglyLinkedList::~SinglyLinkedList()
{
    Node *current = head;
    while (current != nullptr)
    {
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    size = 0;
}

// Chuyển dữ liệu từ vector data vào cấu trúc danh sách liên kết đơn
void SinglyLinkedList::transferFromVector(const std::vector<Student> &initialStudents)
{
    // Xóa danh sách hiện tại nếu có
    Node *current = head;
    while (current != nullptr)
    {
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    size = 0;

    // Thêm từng sinh viên vào danh sách
    for (const auto &student : initialStudents)
    {
        add(student);
    }
}

// Thêm sinh viên vào danh sách
bool SinglyLinkedList::add(const Student &student)
{
    // Kiểm tra trùng lặp MSSV
    Node *current = head;
    while (current != nullptr)
    {
        if (current->data.mssv == student.mssv)
        {
            std::cerr << "MSSV đã tồn tại trong danh sách sinh viên.\n";
            return false;
        }
        current = current->next;
    }

    // Tạo node mới
    Node *newNode = new Node(student);

    // Nếu danh sách rỗng, thêm node mới làm head
    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        // Duyệt đến cuối danh sách
        current = head;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        // Thêm node mới vào cuối danh sách
        current->next = newNode;
    }

    size++;
    return true;
}

// Cập nhật thông tin sinh viên trong danh sách
bool SinglyLinkedList::update(const std::string &studentId, const Student &updatedStudent)
{
    Node *current = head;

    while (current != nullptr)
    {
        if (current->data.mssv == studentId)
        {
            // Tạo sinh viên tạm để lưu thông tin cập nhật
            Student tempStudent = updatedStudent;

            // Giữ nguyên thông tin nếu không cập nhật
            if (tempStudent.mssv.empty())
            {
                tempStudent.mssv = current->data.mssv;
            }
            if (tempStudent.ho.empty())
            {
                tempStudent.ho = current->data.ho;
            }
            if (tempStudent.ten.empty())
            {
                tempStudent.ten = current->data.ten;
            }
            if (tempStudent.lop.empty())
            {
                tempStudent.lop = current->data.lop;
            }
            if (tempStudent.diem < 0.0f)
            {
                // If diem is negative (our special marker), don't update it
                tempStudent.diem = current->data.diem;
            }

            // Kiểm tra trùng lặp MSSV nếu đổi MSSV
            if (studentId != tempStudent.mssv)
            {
                Node *checkNode = head;
                while (checkNode != nullptr)
                {
                    if (checkNode != current && checkNode->data.mssv == tempStudent.mssv)
                    {
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
bool SinglyLinkedList::remove(const std::string &studentId)
{
    if (head == nullptr)
    {
        return false;
    }

    // Nếu xóa node đầu
    if (head->data.mssv == studentId)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
        size--;
        return true;
    }

    // Tìm node cần xóa
    Node *current = head;
    while (current->next != nullptr && current->next->data.mssv != studentId)
    {
        current = current->next;
    }

    // Nếu tìm thấy
    if (current->next != nullptr)
    {
        Node *temp = current->next;
        current->next = temp->next;
        delete temp;
        size--;
        return true;
    }

    return false; // Không tìm thấy sinh viên với MSSV đã cung cấp
}

// Lấy danh sách sinh viên điểm thấp nhất
bool SinglyLinkedList::listLowestScoringStudents() const
{
    if (head == nullptr)
    {
        return false; // Danh sách rỗng
    }

    // Tìm điểm thấp nhất
    float lowestScore = head->data.diem;
    Node *current = head->next;

    while (current != nullptr)
    {
        if (current->data.diem < lowestScore)
        {
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

    while (current != nullptr)
    {
        if (current->data.diem == lowestScore)
        {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
                       stt++, current->data.mssv, current->data.ho, current->data.ten, current->data.lop, current->data.diem);
        }
        current = current->next;
    }

    return true;
}

// Lấy danh sách sinh viên điểm cao nhất
bool SinglyLinkedList::listHighestScoringStudents() const
{
    if (head == nullptr)
    {
        return false;
    }

    // Tìm điểm cao nhất
    float highestScore = head->data.diem;
    Node *current = head->next;

    while (current != nullptr)
    {
        if (current->data.diem > highestScore)
        {
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

    while (current != nullptr)
    {
        if (current->data.diem == highestScore)
        {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
                       stt++, current->data.mssv, current->data.ho, current->data.ten, current->data.lop, current->data.diem);
        }
        current = current->next;
    }

    return true;
}

// Tính điểm trung bình của từng lớp
double SinglyLinkedList::calculateAverageScore() const
{
    if (head == nullptr)
    {
        std::cout << "Không có sinh viên nào trong danh sách." << std::endl;
        return 0.0;
    }

    // Map để lưu tổng điểm và số lượng sinh viên của từng lớp
    std::map<std::string, std::pair<double, int>> classStats;

    // Tính tổng điểm và đếm số sinh viên cho từng lớp
    Node *current = head;
    while (current != nullptr)
    {
        const std::string &className = current->data.lop;
        double score = current->data.diem;

        if (classStats.find(className) == classStats.end())
        {
            // Lớp mới, khởi tạo với điểm đầu tiên và số lượng = 1
            classStats[className] = std::make_pair(score, 1);
        }
        else
        {
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

    for (const auto &entry : classStats)
    {
        const std::string &className = entry.first;
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
std::vector<Student> SinglyLinkedList::getAllStudents() const
{
    std::vector<Student> allStudents;
    Node *current = head;

    while (current != nullptr)
    {
        allStudents.push_back(current->data);
        current = current->next;
    }

    return allStudents;
}

// Sort - Chưa triển khai
double SinglyLinkedList::sort(SortAlgorithmType algorithm, SortCriterionType criterion)
{
    // throw std::logic_error("Sort function is not implemented for SinglyLinkedList.");

    auto start = std::chrono::high_resolution_clock::now();
    if (algorithm == SortAlgorithmType::BUBBLE)
    {
        if (head == nullptr || head->next == nullptr)
            return 0.0;
        bool swapped;
        do
        {
            swapped = false;
            Node *current = head;
            while (current->next != nullptr)
            {
                bool swapNeeded = false;
                switch (criterion)
                {
                case SortCriterionType::MSSV:
                    swapNeeded = current->data.mssv > current->next->data.mssv;
                    break;
                case SortCriterionType::HO:
                    swapNeeded = current->data.ho > current->next->data.ho;
                    break;
                case SortCriterionType::TEN:
                    swapNeeded = current->data.ten > current->next->data.ten;
                    break;
                case SortCriterionType::DIEM:
                    swapNeeded = current->data.diem > current->next->data.diem;
                    break;
                }
                if (swapNeeded)
                {
                    std::swap(current->data, current->next->data);
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
    }
    else if (algorithm == SortAlgorithmType::INSERTION)
    {
        if (head == nullptr || head->next == nullptr)
        {
            return 0.0; // Danh sách rỗng hoặc chỉ có một phần tử
        }

        Node *sorted = nullptr;

        Node *current = head;
        while (current != nullptr)
        {
            Node *nextNode = current->next;

            // Insert current into sorted linked list
            if (sorted == nullptr || (criterion == SortCriterionType::MSSV && sorted->data.mssv >= current->data.mssv) ||
                (criterion == SortCriterionType::HO && sorted->data.ho >= current->data.ho) ||
                (criterion == SortCriterionType::TEN && sorted->data.ten >= current->data.ten) ||
                (criterion == SortCriterionType::DIEM && sorted->data.diem >= current->data.diem))
            {
                current->next = sorted;
                sorted = current;
            }
            else
            {
                Node *temp = sorted;
                while (temp->next != nullptr &&
                       ((criterion == SortCriterionType::MSSV && temp->next->data.mssv < current->data.mssv) ||
                        (criterion == SortCriterionType::HO && temp->next->data.ho < current->data.ho) ||
                        (criterion == SortCriterionType::TEN && temp->next->data.ten < current->data.ten) ||
                        (criterion == SortCriterionType::DIEM && temp->next->data.diem < current->data.diem)))
                {
                    temp = temp->next;
                }
                current->next = temp->next;
                temp->next = current;
            }

            current = nextNode;
        }
        head = sorted; // Cập nhật head của danh sách
    }
    else if (algorithm == SortAlgorithmType::SELECTION)
    {
        // Selection sort implementation
        for (Node *i = head; i != nullptr; i = i->next)
        {
            Node *minNode = i;
            for (Node *j = i; j != nullptr; j = j->next)
            {
                bool swapNeeded = false;
                switch (criterion)
                {
                case SortCriterionType::MSSV:
                    swapNeeded = j->data.mssv < minNode->data.mssv;
                    break;
                case SortCriterionType::HO:
                    swapNeeded = j->data.ho < minNode->data.ho;
                    break;
                case SortCriterionType::TEN:
                    swapNeeded = j->data.ten < minNode->data.ten;
                    break;
                case SortCriterionType::DIEM:
                    swapNeeded = j->data.diem < minNode->data.diem;
                    break;
                }
                if (swapNeeded)
                {
                    minNode = j;
                }
            }
            if (minNode != i)
            {
                std::swap(i->data, minNode->data);
            }
        }
    }
    else if (algorithm == SortAlgorithmType::MERGE)
    {
        // Merge hai danh sách đã sắp xếp
        std::function<Node *(Node *, Node *)> merge = [&](Node *left, Node *right) -> Node *
        {
            if (!left)
                return right;
            if (!right)
                return left;

            Node *result = nullptr;
            if ((criterion == SortCriterionType::MSSV && left->data.mssv <= right->data.mssv) ||
                (criterion == SortCriterionType::HO && left->data.ho <= right->data.ho) ||
                (criterion == SortCriterionType::TEN && left->data.ten <= right->data.ten) ||
                (criterion == SortCriterionType::DIEM && left->data.diem <= right->data.diem))
            {
                result = left;
                result->next = merge(left->next, right);
            }
            else
            {
                result = right;
                result->next = merge(left, right->next);
            }
            return result;
        };

        // Merge sort đệ quy
        std::function<Node *(Node *)> mergeSort = [&](Node *node) -> Node *
        {
            if (!node || !node->next)
                return node;

            // Tìm middle node
            Node *slow = node, *fast = node->next;
            while (fast && fast->next)
            {
                slow = slow->next;
                fast = fast->next->next;
            }
            Node *mid = slow->next;
            slow->next = nullptr; // Cắt danh sách

            Node *left = mergeSort(node);
            Node *right = mergeSort(mid);

            return merge(left, right);
        };

        head = mergeSort(head);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count(); // Trả về thời gian chạy (ms)
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