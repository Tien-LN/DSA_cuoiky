#include "Array.hpp"
#include "console_utils.hpp"
#include "enums.hpp"
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>
#include <iomanip>
#include <map>
#include <fmt/core.h>
#include <fmt/format.h>
#include <chrono>

// Constructor
StudentArray::StudentArray() : currentSize(0) {}

// Chuyển dữ liệu từ vector data vào cấu trúc mảng
void StudentArray::transferFromVector(const std::vector<Student> &initialStudents)
{
    if (initialStudents.size() > MAX_STUDENTS)
    {
        throw std::out_of_range("Số lượng sinh viên vượt quá giới hạn cho phép.");
    }

    currentSize = 0;
    for (const auto &student : initialStudents)
    {
        if (currentSize < MAX_STUDENTS)
        {
            students[currentSize++] = student;
        }
        else
        {
            break;
        }
    }
}

// thêm sinh viên vào mảng
bool StudentArray::add(const Student &student)
{
    if (currentSize >= MAX_STUDENTS)
    {
        throw std::out_of_range("Không thể thêm sinh viên: mảng đã đầy.");
    }
    // kiểm tra trùng lặp MSSV
    for (int i = 0; i < currentSize; ++i)
    {
        if (students[i].mssv == student.mssv)
        {
            std::cerr << "MSSV đã tồn tại trong danh sách sinh viên.\n";
            return false;
        }
    }
    students[currentSize++] = student;
    return true;
}

// Cập nhật thông tin sinh viên trong mảng
bool StudentArray::update(const std::string &studentId, const Student &updatedStudent)
{
    for (int i = 0; i < currentSize; ++i)
    {
        if (students[i].mssv == studentId)
        {

            Student tempStudent = updatedStudent;

            if (tempStudent.mssv.empty())
            {
                tempStudent.mssv = students[i].mssv;
            }
            if (tempStudent.ho.empty())
            {
                tempStudent.ho = students[i].ho;
            }
            if (tempStudent.ten.empty())
            {
                tempStudent.ten = students[i].ten;
            }
            if (tempStudent.lop.empty())
            {
                tempStudent.lop = students[i].lop;
            }
            if (tempStudent.diem < 0.0f)
            {
                tempStudent.diem = students[i].diem;
            }

            // nếu người dùng cập nhật cả mssv thì kiểm tra xem có trùng lặp với sinh viên khác không
            if (studentId != tempStudent.mssv)
            {
                for (int j = 0; j < currentSize; ++j)
                {
                    if (i != j && students[j].mssv == tempStudent.mssv)
                    {
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
bool StudentArray::remove(const std::string &studentId)
{
    for (int i = 0; i < currentSize; ++i)
    {
        if (students[i].mssv == studentId)
        {
            for (int j = i; j < currentSize - 1; ++j)
            {
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
bool StudentArray::listLowestScoringStudents() const
{
    if (currentSize == 0)
    {
        return false;
    }

    float lowestScore = students[0].diem;
    for (int i = 1; i < currentSize; i++)
    {
        if (students[i].diem < lowestScore)
        {
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
    for (int i = 0; i < currentSize; i++)
    {
        if (students[i].diem == lowestScore)
        {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
                       i + 1, students[i].mssv, students[i].ho, students[i].ten, students[i].lop, students[i].diem);
        }
    }

    return true;
}

// lấy danh sách sinh viên điểm cao nhất
bool StudentArray::listHighestScoringStudents() const
{
    if (currentSize == 0)
    {
        return false;
    }

    float highestScore = students[0].diem;
    for (int i = 1; i < currentSize; i++)
    {
        if (students[i].diem > highestScore)
        {
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
    for (int i = 0; i < currentSize; i++)
    {
        if (students[i].diem == highestScore)
        {
            fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
                       i + 1, students[i].mssv, students[i].ho, students[i].ten, students[i].lop, students[i].diem);
        }
    }

    return true;
}

// lấy điểm trung bình của lớp
double StudentArray::calculateAverageScore() const
{
    if (currentSize == 0)
    {
        std::cout << "Không có sinh viên nào trong danh sách." << std::endl;
        return 0.0;
    }

    // Map để lưu tổng điểm và số lượng sinh viên của từng lớp
    std::map<std::string, std::pair<double, int>> classStats;

    // Tính tổng điểm và đếm số sinh viên cho từng lớp
    for (int i = 0; i < currentSize; i++)
    {
        const std::string &className = students[i].lop;
        double score = students[i].diem;

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

// lấy tất cả sinh viên trong mảng
std::vector<Student> StudentArray::getAllStudents() const
{
    std::vector<Student> allStudentsVector;
    allStudentsVector.reserve(currentSize);
    for (int i = 0; i < currentSize; ++i)
    {
        allStudentsVector.push_back(students[i]);
    }
    return allStudentsVector;
}

// Sort
double StudentArray::sort(SortAlgorithmType algorithm, SortCriterionType criterion)
{
    // throw std::logic_error("Sort function is not implemented for StudentArray.");
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    if (algorithm == SortAlgorithmType::BUBBLE)
    {
        // Implement bubble sort
        for (int i = 1; i < currentSize; i++)
        {
            for (int j = currentSize - 1; j >= i; j--)
            {
                bool swapNeeded = false;
                switch (criterion)
                {
                case SortCriterionType::MSSV:
                    swapNeeded = students[j - 1].mssv > students[j].mssv;
                    break;
                case SortCriterionType::HO:
                    swapNeeded = students[j - 1].ho > students[j].ho;
                    break;
                case SortCriterionType::TEN:
                    swapNeeded = students[j - 1].ten > students[j].ten;
                    break;
                case SortCriterionType::DIEM:
                    swapNeeded = students[j - 1].diem > students[j].diem;
                    break;
                }
                if (swapNeeded)
                {
                    std::swap(students[j], students[j - 1]);
                }
            }
        }
    }
    else if (algorithm == SortAlgorithmType::INSERTION)
    {
        // Implement insertion sort
        for (int i = 1; i < currentSize; ++i)
        {
            Student key = students[i];
            int j = i - 1;

            while (j >= 0)
            {
                bool swapNeeded = false;
                switch (criterion)
                {
                case SortCriterionType::MSSV:
                    swapNeeded = students[j].mssv > key.mssv;
                    break;
                case SortCriterionType::HO:
                    swapNeeded = students[j].ho > key.ho;
                    break;
                case SortCriterionType::TEN:
                    swapNeeded = students[j].ten > key.ten;
                    break;
                case SortCriterionType::DIEM:
                    swapNeeded = students[j].diem > key.diem;
                    break;
                }
                if (swapNeeded)
                {
                    students[j + 1] = students[j];
                    j--;
                }
                else
                {
                    break;
                }
            }
            students[j + 1] = key;
        }
    }
    else if (algorithm == SortAlgorithmType::QUICK)
    {
        // Implement quick sort
        std::function<void(int, int)> quickSort;
        quickSort = [&](int left, int right)
        {
            if (left < right)
            {
                int pivotIndex = left + (right - left) / 2;
                Student pivot = students[pivotIndex];
                int i = left, j = right;

                while (i <= j)
                {
                    while (true)
                    {
                        bool swapNeeded = false;
                        switch (criterion)
                        {
                        case SortCriterionType::MSSV:
                            swapNeeded = students[i].mssv < pivot.mssv;
                            break;
                        case SortCriterionType::HO:
                            swapNeeded = students[i].ho < pivot.ho;
                            break;
                        case SortCriterionType::TEN:
                            swapNeeded = students[i].ten < pivot.ten;
                            break;
                        case SortCriterionType::DIEM:
                            swapNeeded = students[i].diem < pivot.diem;
                            break;
                        }
                        if (!swapNeeded)
                            break;
                        i++;
                    }
                    while (true)
                    {
                        bool swapNeeded = false;
                        switch (criterion)
                        {
                        case SortCriterionType::MSSV:
                            swapNeeded = students[j].mssv > pivot.mssv;
                            break;
                        case SortCriterionType::HO:
                            swapNeeded = students[j].ho > pivot.ho;
                            break;
                        case SortCriterionType::TEN:
                            swapNeeded = students[j].ten > pivot.ten;
                            break;
                        case SortCriterionType::DIEM:
                            swapNeeded = students[j].diem > pivot.diem;
                            break;
                        }
                        if (!swapNeeded)
                            break;
                        j--;
                    }
                    if (i <= j)
                    {
                        std::swap(students[i], students[j]);
                        i++;
                        j--;
                    }
                }
                quickSort(left, j);
                quickSort(i, right);
            }
        };
        quickSort(0, currentSize - 1);
    }
    else if (algorithm == SortAlgorithmType::SELECTION)
    {
        // Implement selection sort
        for (int i = 0; i < currentSize - 1; ++i)
        {
            int minIndex = i;
            for (int j = i + 1; j < currentSize; ++j)
            {
                bool swapNeeded = false;
                switch (criterion)
                {
                case SortCriterionType::MSSV:
                    swapNeeded = students[j].mssv < students[minIndex].mssv;
                    break;
                case SortCriterionType::HO:
                    swapNeeded = students[j].ho < students[minIndex].ho;
                    break;
                case SortCriterionType::TEN:
                    swapNeeded = students[j].ten < students[minIndex].ten;
                    break;
                case SortCriterionType::DIEM:
                    swapNeeded = students[j].diem < students[minIndex].diem;
                    break;
                }
                if (swapNeeded)
                {
                    minIndex = j;
                }
            }
            std::swap(students[i], students[minIndex]);
        }
    }
    else if (algorithm == SortAlgorithmType::HEAP)
    {
        int n = currentSize;
        std::function<void(int, int)> heapify;
        heapify = [&](int n, int i)
        {
            int largest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            switch (criterion)
            {
            case SortCriterionType::MSSV:
                if (left < n && students[left].mssv > students[largest].mssv)
                    largest = left;
                if (right < n && students[right].mssv > students[largest].mssv)
                    largest = right;
                break;
            case SortCriterionType::HO:
                if (left < n && students[left].ho > students[largest].ho)
                    largest = left;
                if (right < n && students[right].ho > students[largest].ho)
                    largest = right;
                break;
            case SortCriterionType::TEN:
                if (left < n && students[left].ten > students[largest].ten)
                    largest = left;
                if (right < n && students[right].ten > students[largest].ten)
                    largest = right;
                break;
            case SortCriterionType::DIEM:
                if (left < n && students[left].diem > students[largest].diem)
                    largest = left;
                if (right < n && students[right].diem > students[largest].diem)
                    largest = right;
                break;
            }

            if (largest != i)
            {
                std::swap(students[i], students[largest]);
                heapify(n, largest);
            }
        };

        // 1. Xây dựng max-heap
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(n, i);

        // 2. Trích xuất từng phần tử khỏi heap
        for (int i = n - 1; i > 0; i--)
        {
            std::swap(students[0], students[i]); // Đưa phần tử lớn nhất về cuối
            heapify(i, 0);                       // Gọi lại heapify trên phần còn lại
        }
    }
    else if (algorithm == SortAlgorithmType::MERGE)
    {
        function<void(int, int)> mergeSort;
        mergeSort = [&](int left, int right)
        {
            if (left < right)
            {
                int mid = left + (right - left) / 2;
                mergeSort(left, mid);
                mergeSort(mid + 1, right);

                // Merge the two halves
                int n1 = mid - left + 1;
                int n2 = right - mid;

                std::vector<Student> L(n1), R(n2);
                for (int i = 0; i < n1; i++)
                    L[i] = students[left + i];
                for (int j = 0; j < n2; j++)
                    R[j] = students[mid + 1 + j];

                int i = 0, j = 0, k = left;
                while (i < n1 && j < n2)
                {
                    bool swapNeeded = false;
                    switch (criterion)
                    {
                    case SortCriterionType::MSSV:
                        swapNeeded = L[i].mssv <= R[j].mssv;
                        break;
                    case SortCriterionType::HO:
                        swapNeeded = L[i].ho <= R[j].ho;
                        break;
                    case SortCriterionType::TEN:
                        swapNeeded = L[i].ten <= R[j].ten;
                        break;
                    case SortCriterionType::DIEM:
                        swapNeeded = L[i].diem <= R[j].diem;
                        break;
                    }
                    if (swapNeeded)
                    {
                        students[k++] = L[i++];
                    }
                    else
                    {
                        students[k++] = R[j++];
                    }
                }

                while (i < n1)
                    students[k++] = L[i++];
                while (j < n2)
                    students[k++] = R[j++];
            }
        };
        mergeSort(0, currentSize - 1);
    }
    else
    {
        throw std::invalid_argument("Thuật toán sắp xếp không hợp lệ.");
    }
    auto end = high_resolution_clock::now();
    duration<double, std::milli> elapsed = end - start;
    return elapsed.count(); // trả về thời gian chạy (ms)
}

string StudentArray::getFieldByCriterion(const Student &student, SearchCriterionType criterion) const
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

int StudentArray::lowerBoundPrefix(const std::string &prefix, SearchCriterionType criterion) const
{
    int left = 0, right = currentSize - 1, result = currentSize;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        std::string value = getFieldByCriterion(students[mid], criterion);

        if (toLowerString(value).compare(0, prefix.size(), toLowerString(prefix)) >= 0)
        {
            result = mid;
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    return result;
}

int StudentArray::upperBoundPrefix(const std::string &prefix, SearchCriterionType criterion) const
{
    int left = 0, right = currentSize - 1, result = currentSize;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        std::string value = getFieldByCriterion(students[mid], criterion);

        if (toLowerString(value).compare(0, prefix.size(), toLowerString(prefix)) > 0)
        {
            result = mid;
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    return result;
}

// Search
SearchResult StudentArray::search(SearchCriterionType criterion, const std::string &searchTerm, bool reverseName, int searchAlgoChoice) const
{
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    // Tập kết quả trả về
    vector<Student> resultOfSearch;

    // searchAlgoChoice = 1 -> Tìm kiếm nhị phân
    // searchAlgoChoice = 2 -> Tìm kiếm vét cạn
    if (searchAlgoChoice == 1)
    {
        if (criterion == SearchCriterionType::DIEM)
        {
            // xử lý tìm điểm
        }
        else
        {
            int lo = lowerBoundPrefix(searchTerm, criterion);
            int hi = upperBoundPrefix(searchTerm, criterion);

            for (int i = lo; i < hi; i++)
            {
                std::string field = getFieldByCriterion(students[i], criterion);
                if (startsWith(field, searchTerm))
                {
                    resultOfSearch.push_back(students[i]);
                }
            }
        }
    }
    else if (searchAlgoChoice == 2)
    {
        for (int i = 0; i < currentSize; i++)
        {
            const Student &student = students[i];

            switch (criterion)
            {
            case SearchCriterionType::DIEM:
            {
                std::string diem = std::to_string(student.diem);
                if (containsSubString(diem, searchTerm))
                {
                    resultOfSearch.push_back(student);
                }
                break;
            }
            case SearchCriterionType::HO:
                if (containsSubString(student.ho, searchTerm))
                {
                    resultOfSearch.push_back(student);
                }
                break;
            case SearchCriterionType::TEN:
                if (containsSubString(student.ten, searchTerm))
                {
                    resultOfSearch.push_back(student);
                }
                break;
            case SearchCriterionType::MSSV:
                if (containsSubString(student.mssv, searchTerm))
                {
                    resultOfSearch.push_back(student);
                }
                break;
            case SearchCriterionType::LOP:
                if (containsSubString(student.lop, searchTerm))
                {
                    resultOfSearch.push_back(student);
                }
                break;
            }
        }
    }
    else
    {
        throw std::logic_error("Wrong search Types.");
    }
    auto end = high_resolution_clock::now();
    duration<double, std::milli> elapsed = end - start;
    SearchResult result = SearchResult(resultOfSearch, elapsed.count());
    return result;
}