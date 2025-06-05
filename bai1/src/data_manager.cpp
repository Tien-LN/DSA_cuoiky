#include "data_manager.hpp"
#include "student.hpp"
#include "IStudentList.hpp"
#include "types.hpp"
#include "utils.hpp" 
#include "console_utils.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <limits> 
#include <iomanip> 
#include <chrono>  // thư viện giúp đo thời gian
#include <algorithm> 
#include <set>
#include <fmt/core.h>
#include <fmt/format.h> 
#include <sstream> 

// hàm helper để clear input buffer
void clearInputBufferDM() { 
    std::cin.clear(); 
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

// hàm helper để chuyển đổi chuỗi thành chữ thường
std::string toLowerDM(std::string s) { 
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

void displayAllStudents(const IStudentList& studentList) {
    std::vector<Student> students = studentList.getAllStudents();

    if (students.empty()) {
        std::cout << "Danh sách sinh viên rỗng 😴.\n";
        return;
    }
    SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
    std::cout << "\n--- Danh Sách Sinh Viên 🧾 ---\n\n";
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10}\n",
              "STT", "MSSV", "Họ", "Tên", "Lớp", "Điểm");
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cout << std::string(85, '-') << std::endl;     
    for (size_t i = 0; i < students.size(); ++i) {
        const Student& student = students[i];
        fmt::print("{:<5} {:<15} {:<25} {:<15} {:<15} {:<10.2f}\n",
            i + 1, student.mssv, student.ho, student.ten, student.lop, student.diem);
    }
    std::cout << std::string(85, '-') << std::endl; 
}

void addStudent(IStudentList& studentList) {
    Student newStudent;
    std::string tempInput;
    std::vector<std::string> errorMessages;
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    std::cout << "\n--- Thêm Sinh Viên Mới ➕ ---\n\n";
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    
    // MSSV
    do {
        errorMessages.clear();
        std::cout << "\tNhập MSSV: ";
        std::getline(std::cin, newStudent.mssv);
        newStudent.mssv = trim(newStudent.mssv);
        if (newStudent.mssv.empty()) {
            errorMessages.push_back("MSSV không được để trống.");
        }
        for(const auto& msg : errorMessages) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "\t  Lỗi: ";
            SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
            std::cout << msg << std::endl;
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        }
    } while (!errorMessages.empty());    
    
    // Ho
    do {
        errorMessages.clear();
        std::cout << "\tNhập Họ: ";
        std::getline(std::cin, newStudent.ho);
        newStudent.ho = trim(newStudent.ho);
        if (newStudent.ho.empty()) {
            errorMessages.push_back("Họ không được để trống.");
        }
        for(const auto& msg : errorMessages) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "\t  Lỗi: ";
            SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
            std::cout << msg << std::endl;
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        }
    } while (!errorMessages.empty());

    // Ten
    do {
        errorMessages.clear();
        std::cout << "\tNhập Tên: ";
        std::getline(std::cin, newStudent.ten);
        newStudent.ten = trim(newStudent.ten);
        if (newStudent.ten.empty()) {
            errorMessages.push_back("Tên không được để trống.");
        }
        for(const auto& msg : errorMessages) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "\t  Lỗi: ";
            SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
            std::cout << msg << std::endl;
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        }
    } while (!errorMessages.empty());    
    
    // Lop
    do {
        errorMessages.clear();
        std::cout << "\tNhập Lớp: ";
        std::getline(std::cin, newStudent.lop);
        newStudent.lop = trim(newStudent.lop);
        if (newStudent.lop.empty()) {
            errorMessages.push_back("Lớp không được để trống.");
        }
        for(const auto& msg : errorMessages) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "\t  Lỗi: ";
            SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
            std::cout << msg << std::endl;
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        }
    } while (!errorMessages.empty());
    
    // Diem
    do {
        errorMessages.clear();
        std::cout << "\tNhập Điểm (0.0 - 10.0): ";
        std::getline(std::cin, tempInput);
        tempInput = trim(tempInput);
        if (!validateDiem(tempInput, newStudent.diem, errorMessages)) {
        }
        for(const auto& msg : errorMessages) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "\t  Lỗi: ";
            SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
            std::cout << msg << std::endl;
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        }
    } while (!errorMessages.empty());

    if(studentList.add(newStudent)){
        SetConsoleTextAttribute(hConsole, BRIGHT_GREEN_MAIN);
        std::cout << "\nĐã thêm sinh viên thành công!\n";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cout << "\nDanh sách sinh viên mới:\n";
        displayAllStudents(studentList);
    } else {
        SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
        std::cout << "\nThêm sinh viên mới thất bại.";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    }
    
}   

void updateStudent(IStudentList& studentList) {
    std::string studentIdToUpdate;
    std::string tempInput;
    std::vector<std::string> errorMessages;
    
    std::cout << "\nDanh sách sinh viên hiện tại:\n";
    displayAllStudents(studentList);

    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    std::cout << "\n--- Cập Nhật Thông Tin Sinh Viên ✒️ ---\n\n";
    do {
        errorMessages.clear();
        SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
        std::cout << "Nhập MSSV của sinh viên cần cập nhật: ";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::getline(std::cin, studentIdToUpdate);
        studentIdToUpdate = trim(studentIdToUpdate);
        if (studentIdToUpdate.empty()) {
            errorMessages.push_back("MSSV không được để trống.");
        }
        for(const auto& msg : errorMessages) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "\t  Lỗi: ";
            SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
            std::cout << msg << std::endl;
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        }
    } while (!errorMessages.empty()); 

    errorMessages.clear();
    Student studentWithUpdates;
    studentWithUpdates.diem = -1.0f; 
    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
    std::cout << "Nhập thông tin mới, bỏ trống những trường không muốn cập nhật.\n";
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cout << "\tMSSV mới: ";
    std::getline(std::cin, studentWithUpdates.mssv);
    studentWithUpdates.mssv = trim(studentWithUpdates.mssv);
    std::cout << "\tHọ mới: ";
    std::getline(std::cin, studentWithUpdates.ho);
    studentWithUpdates.ho = trim(studentWithUpdates.ho);
    std::cout << "\tTên mới: ";
    std::getline(std::cin, studentWithUpdates.ten);
    studentWithUpdates.ten = trim(studentWithUpdates.ten);
    std::cout << "\tLớp mới: ";
    std::getline(std::cin, studentWithUpdates.lop);
    studentWithUpdates.lop = trim(studentWithUpdates.lop);
    std::cout << "\tĐiểm mới (0.0 - 10.0): ";
    std::getline(std::cin, tempInput);
    tempInput = trim(tempInput);    if (!tempInput.empty()) {
        std::stringstream ss(tempInput);
        float diemMoi;
        ss >> diemMoi;

        while (ss.fail() || !ss.eof() || diemMoi < 0.0f || diemMoi > 10.0f) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "\t   Lỗi:";
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            cout << "điểm không hợp lệ. Vui lòng nhập lại (0.0 - 10.0), hoặc để trống nếu không cập nhật: ";
            std::getline(std::cin, tempInput);
            tempInput = trim(tempInput);

            if (tempInput.empty()) {
                break;
            }

            ss.clear(); 
            ss.str(tempInput);
            ss >> diemMoi;
        }

        if (!tempInput.empty()) {
            studentWithUpdates.diem = diemMoi;
        }
    }

    if (studentList.update(studentIdToUpdate, studentWithUpdates)) {
        SetConsoleTextAttribute(hConsole, BRIGHT_GREEN_MAIN);
        std::cout << "\nĐã cập nhật sinh viên thành công!\n";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cout << "\nDanh sách sinh viên mới:\n";
        displayAllStudents(studentList);
    } else {
        SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
        std::cout << "Không tìm thấy sinh viên với mssv đã cung cấp hoặc quá trình cập nhật đã thất bại.\n";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    }
}

void deleteStudent(IStudentList& studentList) {
    std::string studentIdToDelete;
    std::vector<std::string> errorMessages;
    std::cout << "\nDanh sách sinh viên hiện tại:\n";
    displayAllStudents(studentList);
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    std::cout << "\n--- Xóa Sinh Viên 🗑️ ---\n\n";
    do {
        errorMessages.clear();
        SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
        std::cout << "Nhập MSSV của sinh viên cần cập nhật: ";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::getline(std::cin, studentIdToDelete);
        studentIdToDelete = trim(studentIdToDelete);
        if (studentIdToDelete.empty()) {
            errorMessages.push_back("MSSV không được để trống.");
        }
        for(const auto& msg : errorMessages) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "\t  Lỗi: ";
            SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
            std::cout << msg << std::endl;
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        }
    } while (!errorMessages.empty()); 

    errorMessages.clear();

    if (studentList.remove(studentIdToDelete)) {
        SetConsoleTextAttribute(hConsole, BRIGHT_GREEN_MAIN);
        std::cout << "\nĐã xóa sinh viên thành công!\n";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cout << "\nDanh sách sinh viên mới:\n";
        displayAllStudents(studentList);
    } else {
        SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
        std::cout << "Không tìm thấy sinh viên với mssv đã cung cấp hoặc quá trình xóa đã thất bại..\n";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    }
}

void listLowestScoringStudents(const IStudentList& studentList) {
    if (!studentList.listLowestScoringStudents()) {
        std::cout << "\tKhông có sinh viên hoặc không thể liệt kê sinh viên có điểm thấp nhất.\n";
    }
}

void listHighestScoringStudents(const IStudentList& studentList) {
    if (!studentList.listHighestScoringStudents()) {
        std::cout << "\tKhông có sinh viên hoặc không thể liệt kê sinh viên có điểm cao nhất.\n";
    }
}

double calculateAverageScore(const IStudentList& studentList) {
    double avgScore = studentList.calculateAverageScore();
    return avgScore;
}

void sortStudents(IStudentList& studentList, isSorted& dataSorted) {
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    std::cout << "\n--- Sắp Xếp Danh Sách Sinh Viên 📏 ---\n\n";
    int algoChoice = 0, criterionChoice = 0;
    
    // Algorithm selection
    bool validAlgoChoice = false;
    while (!validAlgoChoice) {
        SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
        std::cout << "Chọn thuật toán sắp xếp:\n\n";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cout << "\t1. Bubble Sort\n";
        std::cout << "\t2. Insertion Sort\n";
        std::cout << "\t3. Quick Sort\n";
        std::cout << "\t4. Selection Sort\n";
        std::cout << "\t5. Heap Sort\n";
        std::cout << "\t6. Merge Sort\n";
        std::cout << "\t0. Quay lại menu chính\n";
        SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
        std::cout << "\nLựa chọn của bạn (0-6): ";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cin >> algoChoice;
        
        if (algoChoice == 0) {
            clearInputBufferDM();
            return; // Return to main menu
        }
        
        if (std::cin.fail() || algoChoice < 0 || algoChoice > 6) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "\tLựa chọn không hợp lệ. Vui lòng chọn lại.\n";
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            clearInputBufferDM();
            continue;
        }
        
        validAlgoChoice = true;
        clearInputBufferDM();
    }
    
    SortAlgorithmType algorithm;
    switch (algoChoice) {
        case 1: algorithm = SortAlgorithmType::BUBBLE; break;
        case 2: algorithm = SortAlgorithmType::INSERTION; break;
        case 3: algorithm = SortAlgorithmType::QUICK; break;
        case 4: algorithm = SortAlgorithmType::SELECTION; break;
        case 5: algorithm = SortAlgorithmType::HEAP; break;
        case 6: algorithm = SortAlgorithmType::MERGE; break;
        default: 
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "   Lỗi";
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            std::cout << "Lựa chọn thuật toán không hợp lệ.\n"; 
            return;
    }    // Criterion selection
    bool validCriterionChoice = false;
    while (!validCriterionChoice) {
        SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
        std::cout << "Chọn tiêu chí sắp xếp:\n";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cout << "\t1. MSSV\n";
        std::cout << "\t2. Họ\n";
        std::cout << "\t3. Tên\n";
        std::cout << "\t4. Điểm\n";
        std::cout << "\t0. Quay lại menu chính\n";
        SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
        std::cout << "Lựa chọn của bạn (0-4): ";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cin >> criterionChoice;
        
        if (criterionChoice == 0) {
            clearInputBufferDM();
            return;
        }
        
        if (std::cin.fail() || criterionChoice < 0 || criterionChoice > 4) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "   Lỗi: ";
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            std::cout << "Lựa chọn không hợp lệ. Vui lòng chọn lại.\n";
            clearInputBufferDM();
            continue;
        }
        
        validCriterionChoice = true;
        clearInputBufferDM();
    }
    
    SortCriterionType criterion;
    switch (criterionChoice) {
        case 1: criterion = SortCriterionType::MSSV; break;
        case 2: criterion = SortCriterionType::HO; break; 
        case 3: criterion = SortCriterionType::TEN; break;
        case 4: criterion = SortCriterionType::DIEM; break;
        default: 
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "   Lỗi";
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            std::cout << "Lựa chọn tiêu chí không hợp lệ.\n"; 
            return;
    }

    std::cout << "Danh sách sinh viên hiện tại:\n";
    displayAllStudents(studentList);
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    std::cout << "Đang sắp xếp..." << std::endl;
    double timeTaken = studentList.sort(algorithm, criterion);
    SetConsoleTextAttribute(hConsole, BRIGHT_GREEN_MAIN);
    std::cout << "Đã sắp xếp xong.\n";

    switch (criterion)
    {
    case SortCriterionType::DIEM : dataSorted = isSorted::DIEM;break;
    case SortCriterionType::HO : dataSorted = isSorted::HO;break;
    case SortCriterionType::TEN : dataSorted = isSorted::TEN;break;
    case SortCriterionType::MSSV : dataSorted = isSorted::MSSV;break;
    default:
        break;
    }

    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cout << "Thời gian thực hiện: "; 
    SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
    std::cout << std::fixed << std::setprecision(3) << timeTaken << " ms\n";
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cout << "Danh sách sinh viên sau khi sắp xếp:\n";
    
    displayAllStudents(studentList);
}


void searchStudents(IStudentList& studentList, isSorted dataSorted, DataStructureType dataStructureType) {
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    std::cout << "\n--- Tìm Kiếm Sinh Viên 🔍 ---\n\n";
    int searchAlgoChoice = 0, criterionChoice = 0;
    
    // Algorithm selection
    bool validAlgoChoice = false;
    while (!validAlgoChoice) {
        SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
        std::cout << "Chọn thuật toán tìm kiếm:\n\n";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cout << "\t1. Tìm kiếm nhị phân (yêu cầu danh sách đã sắp xếp theo tiêu chí tìm kiếm)\n";
        std::cout << "\t2. Tìm kiếm tuần tự\n";
        std::cout << "\t0. Quay lại menu chính\n";
        SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
        std::cout << "\nLựa chọn của bạn (0-2): ";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cin >> searchAlgoChoice;
        
        if (searchAlgoChoice == 0) {
            clearInputBufferDM();
            return; // Return to main menu
        }
        
        if (std::cin.fail() || searchAlgoChoice < 0 || searchAlgoChoice > 2) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "\tLựa chọn không hợp lệ. Vui lòng chọn lại.\n";
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            clearInputBufferDM();
            continue;
        }

        if (searchAlgoChoice == 1 && dataSorted == isSorted::NONE) {
            searchAlgoChoice = 0;
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "\tDanh sách tìm kiếm chưa được sắp xếp hãy sắp xếp lại trước khi tìm kiếm nhị phân.\n";
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            clearInputBufferDM();
            continue;
        }

        // Kiểm tra nếu đang tìm kiếm nhị phân trên danh sách liên kết đơn, vòng không
        if (searchAlgoChoice == 1) {
            bool IsNoValid = 
                (dataStructureType == DataStructureType::CIRCULAR_LINKED_LIST) || 
                (dataStructureType == DataStructureType::SINGLY_LINKED_LIST);
            if (IsNoValid) {
                searchAlgoChoice = 0;
                SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                std::cout << "\tDanh sách tìm kiếm không phù hợp.\n";
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                clearInputBufferDM();
                continue;
            }
        }
        
        validAlgoChoice = true;
        clearInputBufferDM();
    }
    
    // Criterion selection
    bool validCriterionChoice = false;
    while (!validCriterionChoice) {
        SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
        std::cout << "Chọn tiêu chí tìm kiếm:\n";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cout << "\t1. MSSV\n";
        std::cout << "\t2. Họ\n";
        std::cout << "\t3. Tên\n";
        std::cout << "\t4. Lớp\n";
        std::cout << "\t5. Điểm\n";
        std::cout << "\t0. Quay lại menu chính\n";
        SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
        std::cout << "Lựa chọn của bạn (0-5): ";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cin >> criterionChoice;
        
        if (criterionChoice == 0) {
            clearInputBufferDM();
            return;
        }
        
        if (std::cin.fail() || criterionChoice < 0 || criterionChoice > 5) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "   Lỗi: ";
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            std::cout << "Lựa chọn không hợp lệ. Vui lòng chọn lại.\n";
            clearInputBufferDM();
            continue;
        }
        
        // Nếu là tìm kiếm nhị phân thì kiểm tra xem nó đã sắp xếp theo đúng tiêu chí tìm kiếm chưa
        if (searchAlgoChoice == 1) {
            bool isMatched = 
                (criterionChoice == 1 && dataSorted == isSorted::MSSV) ||
                (criterionChoice == 2 && dataSorted == isSorted::HO)   ||
                (criterionChoice == 3 && dataSorted == isSorted::TEN)  ||
                (criterionChoice == 4 && dataSorted == isSorted::LOP)  ||
                (criterionChoice == 5 && dataSorted == isSorted::DIEM);
            
            if(!isMatched){
                SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                std::cout << "Dữ liệu hiện tại không được sắp theo tiêu chí đã chọn. Vui lòng sắp xếp lại hoặc chọn tiêu chí khác.\n";
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                criterionChoice = 0;
                clearInputBufferDM();
                continue; // yêu cầu chọn lại
            }
        }

        validCriterionChoice = true;
        clearInputBufferDM();
    }
    
    SearchCriterionType criterion;
    switch (criterionChoice) {
        case 1: criterion = SearchCriterionType::MSSV; break;
        case 2: criterion = SearchCriterionType::HO; break; 
        case 3: criterion = SearchCriterionType::TEN; break;
        case 4: criterion = SearchCriterionType::LOP; break;
        case 5: criterion = SearchCriterionType::DIEM; break;
        default: 
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            std::cout << "   Lỗi";
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            std::cout << "Lựa chọn tiêu chí không hợp lệ.\n"; 
            return;
    }
    
    // Hiển thị yêu cầu đảo ngược tên
    bool reverseName = false;
    char reverseChoice;
    SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
    std::cout << "Bạn có muốn hiển thị tên đảo ngược không? (y/n): ";
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::cin >> reverseChoice;
    clearInputBufferDM();
    
    if (reverseChoice == 'y' || reverseChoice == 'Y') {
        reverseName = true;
    }
    
    
    // Nhập giá trị tìm kiếm
    std::string searchValue;
    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
    std::cout << "Nhập giá trị cần tìm kiếm: ";
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    std::getline(std::cin, searchValue);
    searchValue = trim(searchValue);
    
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    std::cout << "Đang tìm kiếm..." << std::endl;
    
    // Thực hiện tìm kiếm
    SearchResult result = studentList.search(criterion, searchValue, reverseName, searchAlgoChoice);
    
    SetConsoleTextAttribute(hConsole, BRIGHT_GREEN_MAIN);
    std::cout << "Tìm kiếm hoàn tất.\n";
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    
    std::cout << "Thời gian thực hiện: "; 
    SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
    std::cout << std::fixed << std::setprecision(3) << result.timeTakenMilliseconds << " ms\n";
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    
    // Hiển thị kết quả tìm kiếm
    std::cout << "Kết quả tìm kiếm (" << result.students.size() << " sinh viên):\n";
    
    if (result.students.empty()) {
        SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
        std::cout << "Không tìm thấy sinh viên nào phù hợp với tiêu chí tìm kiếm.\n";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
    } else {
        // Display results with highlighted search term
        SetConsoleTextAttribute(hConsole, BRIGHT_VIOLET_MAIN);
        std::cout << "\n--- Kết Quả Tìm Kiếm 🔍 ---\n\n";
        SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
        fmt::print("{:<5} {:<15} {:<15} {:<15} {:<15} {:<10}", 
                 "STT", "MSSV", "Họ", "Tên", "Lớp", "Điểm");
        
        if (reverseName) {
            std::cout << " | Tên đảo ngược";
        }
        
        std::cout << std::endl;
        
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        std::cout << std::string(85 + (reverseName ? 25 : 0), '-') << std::endl;
        
        for (size_t i = 0; i < result.students.size(); ++i) {
            const Student& student = result.students[i];
            
            // In ra thông tin cơ bản
            fmt::print("{:<5} ", i + 1);

            if (criterion == SearchCriterionType::MSSV) {
                SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                fmt::print("{:<15} ", student.mssv);
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                fmt::print("{:<25} {:<15} {:<15} {:<10.2f}", 
                         student.ho, student.ten, student.lop, student.diem);
            } else if (criterion == SearchCriterionType::HO) {
                fmt::print("{:<15} ", student.mssv);
                SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                fmt::print("{:<25} ", student.ho);
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                fmt::print("{:<15} {:<15} {:<10.2f}", 
                         student.ten, student.lop, student.diem);
            } else if (criterion == SearchCriterionType::TEN) {
                fmt::print("{:<15} {:<25} ", student.mssv, student.ho);
                SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                fmt::print("{:<15} ", student.ten);
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                fmt::print("{:<15} {:<10.2f}", student.lop, student.diem);
            } else if (criterion == SearchCriterionType::LOP) {
                fmt::print("{:<15} {:<25} {:<15} ", student.mssv, student.ho, student.ten);
                SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                fmt::print("{:<15} ", student.lop);
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                fmt::print("{:<10.2f}", student.diem);
            } else if (criterion == SearchCriterionType::DIEM) {
                fmt::print("{:<15} {:<25} {:<15} {:<15} ", 
                         student.mssv, student.ho, student.ten, student.lop);
                SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                fmt::print("{:<10.2f}", student.diem);
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            }            
            if (reverseName) {
                std::string fullName = student.ten + " " + student.ho;
                std::string reversedName;
                for (size_t j = fullName.length() - 1; j >= 0; j--) {
                    reversedName += fullName[j];
                }
                SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
                std::cout << " | " << reversedName;
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            }
            
            std::cout << std::endl;
        }
        std::cout << std::string(85 + (reverseName ? 25 : 0), '-') << std::endl;
    }
}

