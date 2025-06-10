#include "DoublyLinkedList.hpp"
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
DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

// Destructor
DoublyLinkedList::~DoublyLinkedList()
{
    Node *current = head;
    while (current != nullptr)
    {
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
        if (current == head)
            break; 
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Chuyển dữ liệu từ vector data vào cấu trúc danh sách liên kết
void DoublyLinkedList::transferFromVector(const std::vector<Student> &initialStudents)
{
    // Xóa danh sách hiện tại nếu có
    Node *current = head;
    while (current != nullptr)
    {
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
        if (current == head)
            break; 
    }
    head = nullptr;
    tail = nullptr;
    size = 0;

    // Thêm từng sinh viên vào danh sách
    for (const auto &student : initialStudents)
    {
        add(student);
    }
}

// Thêm sinh viên vào danh sách
bool DoublyLinkedList::add(const Student &student)
{
    // Kiểm tra trùng lặp MSSV
    Node *current = head;
    for (int i = 0; i < size; i++)
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

    // Nếu danh sách rỗng
    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        // Thêm vào cuối danh sách
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }

    size++;
    return true;
}

// Cập nhật thông tin sinh viên trong danh sách
bool DoublyLinkedList::update(const std::string &studentId, const Student &updatedStudent)
{
    Node *current = head;

    for (int i = 0; i < size; i++)
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
                for (int j = 0; j < size; j++)
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
bool DoublyLinkedList::remove(const std::string &studentId)
{
    if (head == nullptr)
    {
        return false; // Danh sách rỗng
    }

    Node *current = head;

    for (int i = 0; i < size; i++)
    {
        if (current->data.mssv == studentId)
        {
            // Nếu chỉ có một node
            if (size == 1)
            {
                delete current;
                head = nullptr;
                tail = nullptr;
            }
            // Nếu là node đầu tiên
            else if (current == head)
            {
                head = head->next;
                head->prev = nullptr;
                delete current;
            }
            // Nếu là node cuối cùng
            else if (current == tail)
            {
                tail = tail->prev;
                tail->next = nullptr;
                delete current;
            }
            // Nếu là node ở giữa
            else
            {
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
bool DoublyLinkedList::listLowestScoringStudents() const
{
    if (head == nullptr)
    {
        return false; // Danh sách rỗng
    }

    // Tìm điểm thấp nhất
    float lowestScore = head->data.diem;
    Node *current = head->next;

    for (int i = 1; i < size; i++)
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

    for (int i = 0; i < size; i++)
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
bool DoublyLinkedList::listHighestScoringStudents() const
{
    if (head == nullptr)
    {
        return false; // Danh sách rỗng
    }

    // Tìm điểm cao nhất
    float highestScore = head->data.diem;
    Node *current = head->next;

    for (int i = 1; i < size; i++)
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

    for (int i = 0; i < size; i++)
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
double DoublyLinkedList::calculateAverageScore() const
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
    for (int i = 0; i < size; i++)
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
std::vector<Student> DoublyLinkedList::getAllStudents() const
{
    std::vector<Student> allStudents;
    Node *current = head;

    for (int i = 0; i < size; i++)
    {
        allStudents.push_back(current->data);
        current = current->next;
    }

    return allStudents;
}

// Sort - Chưa triển khai
double DoublyLinkedList::sort(SortAlgorithmType algorithm, SortCriterionType criterion)
{

    auto start = std::chrono::high_resolution_clock::now();
    if (algorithm == SortAlgorithmType::BUBBLE)
    {
        if (head == nullptr || head->next == nullptr)
        {
            return 0.0; // Danh sách rỗng hoặc chỉ có một phần tử
        }
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
            return 0.0;
        }
        Node *current = head->next;
        while (current != nullptr)
        {
            Node *keyNode = current;
            Node *prev = current->prev;
            // Lưu lại dữ liệu cần chèn
            Student keyData = keyNode->data;
            // Dịch chuyển các phần tử lớn hơn về sau
            while (prev != nullptr)
            {
                bool swapNeeded = false;
                switch (criterion)
                {
                case SortCriterionType::MSSV:
                    swapNeeded = prev->data.mssv > keyData.mssv;
                    break;
                case SortCriterionType::HO:
                    swapNeeded = prev->data.ho > keyData.ho;
                    break;
                case SortCriterionType::TEN:
                    swapNeeded = prev->data.ten > keyData.ten;
                    break;
                case SortCriterionType::DIEM:
                    swapNeeded = prev->data.diem > keyData.diem;
                    break;
                }
                if (swapNeeded)
                {
                    prev->next->data = prev->data;
                    prev = prev->prev;
                }
                else
                {
                    break;
                }
            }
            if (prev == nullptr)
            {
                head->data = keyData;
            }
            else
            {
                prev->next->data = keyData;
            }
            current = current->next;
        }
    }
    else if (algorithm == SortAlgorithmType::SELECTION)
    {
        if (head == nullptr || head->next == nullptr)
        {
            return 0.0;
        }
        for (Node *i = head; i != nullptr; i = i->next)
        {
            Node *minNode = i;
            for (Node *j = i->next; j != nullptr; j = j->next)
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
    else if (algorithm == SortAlgorithmType::QUICK)
    {
        if (head == nullptr || head->next == nullptr)
            return 0.0;

        // Hàm partition
        auto partition = [&](Node *low, Node *high) -> Node *
        {
            Student pivot = high->data;
            Node *i = low->prev;

            for (Node *j = low; j != high; j = j->next)
            {
                bool less = false;
                switch (criterion)
                {
                case SortCriterionType::MSSV:
                    less = j->data.mssv < pivot.mssv;
                    break;
                case SortCriterionType::HO:
                    less = j->data.ho < pivot.ho;
                    break;
                case SortCriterionType::TEN:
                    less = j->data.ten < pivot.ten;
                    break;
                case SortCriterionType::DIEM:
                    less = j->data.diem < pivot.diem;
                    break;
                }
                if (less)
                {
                    i = (i == nullptr) ? low : i->next;
                    std::swap(i->data, j->data);
                }
            }
            i = (i == nullptr) ? low : i->next;
            std::swap(i->data, high->data);
            return i;
        };

        // Hàm quicksort đệ quy
        std::function<void(Node *, Node *)> quickSort = [&](Node *low, Node *high)
        {
            if (high != nullptr && low != high && low != high->next)
            {
                Node *p = partition(low, high);
                quickSort(low, p->prev);
                quickSort(p->next, high);
            }
        };

        quickSort(head, tail);
    }
    else if (algorithm == SortAlgorithmType::HEAP)
    {
        if (head == nullptr || head->next == nullptr)
            return 0.0;

        // Hàm lấy node theo chỉ số
        auto getNodeAt = [&](int index) -> Node *
        {
            Node *current = head;
            for (int i = 0; i < index && current; ++i)
                current = current->next;
            return current;
        };

        // Hàm heapify
        std::function<void(int, int)> heapify = [&](int n, int i)
        {
            int largest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            Node *nodeLargest = getNodeAt(largest);
            Node *nodeLeft = getNodeAt(left);
            Node *nodeRight = getNodeAt(right);

            switch (criterion)
            {
            case SortCriterionType::MSSV:
                if (left < n && nodeLeft->data.mssv > nodeLargest->data.mssv)
                    largest = left;
                if (right < n && nodeRight->data.mssv > getNodeAt(largest)->data.mssv)
                    largest = right;
                break;
            case SortCriterionType::HO:
                if (left < n && nodeLeft->data.ho > nodeLargest->data.ho)
                    largest = left;
                if (right < n && nodeRight->data.ho > getNodeAt(largest)->data.ho)
                    largest = right;
                break;
            case SortCriterionType::TEN:
                if (left < n && nodeLeft->data.ten > nodeLargest->data.ten)
                    largest = left;
                if (right < n && nodeRight->data.ten > getNodeAt(largest)->data.ten)
                    largest = right;
                break;
            case SortCriterionType::DIEM:
                if (left < n && nodeLeft->data.diem > nodeLargest->data.diem)
                    largest = left;
                if (right < n && nodeRight->data.diem > getNodeAt(largest)->data.diem)
                    largest = right;
                break;
            }

            if (largest != i)
            {
                std::swap(getNodeAt(i)->data, getNodeAt(largest)->data);
                heapify(n, largest);
            }
        };

        // Xây dựng heap
        for (int i = size / 2 - 1; i >= 0; --i)
            heapify(size, i);

        // Trích xuất từng phần tử khỏi heap
        for (int i = size - 1; i > 0; --i)
        {
            std::swap(getNodeAt(0)->data, getNodeAt(i)->data);
            heapify(i, 0);
        }
    }
    else if (algorithm == SortAlgorithmType::MERGE)
    {
        if (head == nullptr || head->next == nullptr)
            return 0.0;

        // Hàm merge hai danh sách đã sắp xếp
        std::function<Node *(Node *, Node *)> merge = [&](Node *left, Node *right) -> Node *
        {
            if (!left)
                return right;
            if (!right)
                return left;

            if (
                (criterion == SortCriterionType::MSSV && left->data.mssv <= right->data.mssv) ||
                (criterion == SortCriterionType::HO && left->data.ho <= right->data.ho) ||
                (criterion == SortCriterionType::TEN && left->data.ten <= right->data.ten) ||
                (criterion == SortCriterionType::DIEM && left->data.diem <= right->data.diem))
            {
                left->next = merge(left->next, right);
                if (left->next)
                    left->next->prev = left;
                left->prev = nullptr;
                return left;
            }
            else
            {
                right->next = merge(left, right->next);
                if (right->next)
                    right->next->prev = right;
                right->prev = nullptr;
                return right;
            }
        };

        // Hàm merge sort đệ quy
        std::function<Node *(Node *)> mergeSort = [&](Node *node) -> Node *
        {
            if (!node || !node->next)
                return node;

            // Tìm middle node
            Node *slow = node;
            Node *fast = node;
            while (fast->next && fast->next->next)
            {
                slow = slow->next;
                fast = fast->next->next;
            }
            Node *mid = slow->next;
            slow->next = nullptr;
            if (mid)
                mid->prev = nullptr;

            Node *left = mergeSort(node);
            Node *right = mergeSort(mid);

            return merge(left, right);
        };

        head = mergeSort(head);

        // Cập nhật lại tail
        tail = head;
        while (tail && tail->next)
            tail = tail->next;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count(); // Trả về thời gian chạy (ms)
}

// lấy phần tử dựa trên criterion
string DoublyLinkedList::getFieldByCriterion(const Student &student, SearchCriterionType criterion) const
{
    switch (criterion)
    {
    case SearchCriterionType::MSSV:
        return student.mssv;
    case SearchCriterionType::HO:
        return student.ho;
    case SearchCriterionType::TEN:
        return student.ten;
    case SearchCriterionType::LOP:
        return student.lop;
    case SearchCriterionType::DIEM:
        return std::to_string(student.diem);
    default:
        return "";
    }
}

// Search
SearchResult DoublyLinkedList::search(SearchCriterionType criterion, const std::string &searchTerm, bool reverseName, int searchAlgoChoice) const
{
    using namespace std::chrono;
    std::vector<Student> resultOfSearch;

    auto start = high_resolution_clock::now();

    // searchAlgoChoice = 1 -> Tìm kiếm nhị phân
    // searchAlgoChoice = 2 -> Tìm kiếm vét cạn
    if (searchAlgoChoice == 1)
    {
        // Số lượng tối đa có thể có của danh sách
        const int MAX_STUDENTS = 1000;
        // Đánh dấu cho các nốt bằng thứ tự để kiểm soát truy cập nhanh chóng
        Node *node_indexed[MAX_STUDENTS];

        Node *current = head;
        for (int i = 0; i < size; i++)
        {
            node_indexed[i + 1] = current; // 1 -> size
            current = current->next;
        }

        for (int i = size + 1; i < MAX_STUDENTS; i++)
        {
            node_indexed[i] = nullptr;
        }

        // khai báo cận dưới với cận trên
        Node *lo = nullptr;
        Node *hi = nullptr;

        // tìm cận dưới thỏa [cận dưới, cận trên)
        int l = 1, r = size;
        while (l <= r)
        {
            int mid = (l + r) >> 1;
            Node *node_process = node_indexed[mid];
            const Student &student = node_process->data;
            string value = getFieldByCriterion(student, criterion);

            if (toLowerString(value).compare(0, searchTerm.size(), toLowerString(searchTerm)) >= 0)
            {
                lo = node_process;
                r = mid - 1;
            }
            else
            {
                l = mid + 1;
            }
        }

        // tìm cận trên
        l = 1, r = size;
        while (l <= r)
        {
            int mid = (l + r) >> 1;
            Node *node_process = node_indexed[mid];
            const Student &student = node_process->data;
            string value = getFieldByCriterion(student, criterion);

            if (toLowerString(value).compare(0, searchTerm.size(), toLowerString(searchTerm)) > 0)
            {
                hi = node_process;
                r = mid - 1;
            }
            else
            {
                l = mid + 1;
            }
        }

        if (lo != nullptr)
        {
            current = lo;
            while (current != hi && current != nullptr)
            {
                const Student &student = current->data;
                const string value = getFieldByCriterion(student, criterion);
                if (startsWith(value, searchTerm))
                {
                    resultOfSearch.push_back(student);
                }
                current = current->next;
            }
        }

        auto end = high_resolution_clock::now();
        duration<double, std::milli> elapsed = end - start;
        SearchResult result = SearchResult(resultOfSearch, elapsed.count());
        return result;
    }
    else
    {
        Node *current = head;

        do
        {
            const Student &student = current->data;
            bool isMatched = false;

            switch (criterion)
            {
            case SearchCriterionType::DIEM:
            {
                std::string diem = std::to_string(student.diem);
                isMatched = containsSubString(diem, searchTerm);
                break;
            }
            case SearchCriterionType::HO:
                isMatched = containsSubString(student.ho, searchTerm);
                break;
            case SearchCriterionType::TEN:
                isMatched = containsSubString(student.ten, searchTerm);
                break;
            case SearchCriterionType::MSSV:
                isMatched = containsSubString(student.mssv, searchTerm);
                break;
            case SearchCriterionType::LOP:
                isMatched = containsSubString(student.lop, searchTerm);
                break;
            }

            if (isMatched)
            {
                resultOfSearch.push_back(student);
            }

            current = current->next;
        } while (current != nullptr);

        auto end = high_resolution_clock::now();
        duration<double, std::milli> elapsed = end - start;
        SearchResult result = SearchResult(resultOfSearch, elapsed.count());
        return result;
    }
}