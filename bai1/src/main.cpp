#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "student.hpp"
#include "data_manager.hpp" 
#include "file_handler.hpp"
#include "enums.hpp"
#include "menu.hpp"
#include "Array.hpp" 
#include "SinglyLinkedList.hpp" 
#include "DoublyLinkedList.hpp"
#include "CircularLinkedList.hpp"
#include "console_utils.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN // Add this to reduce the scope of windows.h
#include <windows.h> // For SetConsoleOutputCP
#endif

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Set console output to UTF-8
    SetConsoleCP(CP_UTF8);       // Set console input to UTF-8
#endif
    // No need to get console handle here, it's already defined in console_utils

    vector<Student> students;
    IStudentList* studentList = nullptr;
    DataStructureType dataStructureType = DataStructureType::ARRAY; 
    bool dataLoaded = false;
    bool hasInvalidRows = false;
    bool dataStructureSelected = false;
    string dsName;
    isSorted dataSorted = isSorted::NONE;

    int choice;

    do {
        displayMenu(dataLoaded, hasInvalidRows, dataStructureSelected, dsName);

        SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
        cout << "Nhập lựa chọn của bạn: ";
        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN); 
        
        // Kiểm tra đầu vào
        if (!(cin >> choice)) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            cout << "Lựa chọn không hợp lệ. Vui lòng nhập số.\n";
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);            
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }
        
        cin.ignore(); // Đọc bỏ ký tự newline còn lại trong buffer

        // kiểm tra lựa chọn
        if (!dataLoaded && choice != 1 && choice != 0) {
            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
            cout << "Vui lòng tải dữ liệu trước khi thực hiện các thao tác khác." << endl;
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
            cout << "Nhấn Enter để tiếp tục...";
            cin.get();
            continue;
        }

        switch (choice) {
            case 1: {
                string filename_input;
                // Case Input File set Sorted = false 
                dataSorted = isSorted::NONE;
                SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
                cout << "Nhập tên file dữ liệu (ví dụ: false_data.csv, file này phải nằm trong thư mục 'data'): "; // Updated prompt
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                getline(cin, filename_input);
                string full_filepath = "./../data/" + filename_input;
                cout << "Đang thử mở file tại đường dẫn: " << full_filepath << endl;
                hasInvalidRows = false; 
                students.clear();                
                if (importDataFromFile(full_filepath, students, hasInvalidRows)) {
                    if (!students.empty()) {
                        dataLoaded = true;
                        if (dataStructureSelected && studentList != nullptr) {
                            cout << "Đang cập nhật dữ liệu trong cấu trúc " << dsName << "..." << endl;
                            studentList->transferFromVector(students);
                            cout << "Cập nhật dữ liệu thành công!" << endl;
                        }
                        
                        SetConsoleTextAttribute(hConsole, BRIGHT_GREEN_MAIN);
                        cout << "Dữ liệu đã được tải thành công." << endl;
                        SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
                        if (hasInvalidRows) {                          
                            cout << "Vui lòng sửa các dòng không hợp lệ trong file data rồi import lại hoặc import file data hợp lệ khác để có thể thực hiện các chức năng khác của chương trình!" << endl;
                        } else {
                            cout << "Tất cả dữ liệu đều hợp lệ. Bạn có thể thực hiện các chức năng khác của chương trình!" << endl;
                        }
                        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                    } else {
                        SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                        cout << "File đã được xử lý, nhưng không có dữ liệu sinh viên hợp lệ nào được tải." << endl;
                        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                    }
                }
                break;
            }
            case 2: {
                int dsType;
                SetConsoleTextAttribute(hConsole, BRIGHT_GREEN_MAIN);
                cout << "\tChọn cấu trúc dữ liệu:\n\n";
                SetConsoleTextAttribute(hConsole, BRIGHT_PURPLE_MAIN);
                cout << "\t\t1. Mảng\n\n";
                cout << "\t\t2. Danh sách liên kết đơn\n\n";
                cout << "\t\t3. Danh sách liên kết vòng\n\n";
                cout << "\t\t4. Danh sách liên kết kép\n\n";
                SetConsoleTextAttribute(hConsole, BRIGHT_GREEN_MAIN);
                cout << "\tLựa chọn: ";
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                
                if (!(cin >> dsType)) {
                    SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                    cout << "\tLựa chọn không hợp lệ. Vui lòng nhập số.\n";
                    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);                    
                    cin.clear();
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    break;
                }
                
                cin.ignore(); // Clear input buffer
                if (studentList != nullptr) {
                    students = studentList->getAllStudents();
                    delete studentList;
                    studentList = nullptr;
                }

                switch (dsType) {
                    case 1:
                        dataStructureType = DataStructureType::ARRAY;
                        studentList = new StudentArray(); 
                        dataStructureSelected = true;
                        dsName = "Mảng";
                        break;                    
                    case 2:
                        dataStructureType = DataStructureType::SINGLY_LINKED_LIST;
                        studentList = new SinglyLinkedList();
                        dataStructureSelected = true;
                        dsName = "Danh sách liên kết đơn";
                        break;                    
                    case 3:
                        dataStructureType = DataStructureType::CIRCULAR_LINKED_LIST;
                        studentList = new CircularLinkedList();
                        dataStructureSelected = true;
                        dsName = "Danh sách liên kết vòng";
                        break;                    
                    case 4:
                        dataStructureType = DataStructureType::DOUBLY_LINKED_LIST;
                        studentList = new DoublyLinkedList();
                        dataStructureSelected = true;
                        dsName = "Danh sách liên kết kép";
                        break;
                    default:
                        SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                        cout << "\tLựa chọn không hợp lệ." << endl;
                        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                        break;
                }

                SetConsoleTextAttribute(hConsole, BRIGHT_GREEN_MAIN);
                cout << "\tBạn đã chọn lưu trữ dữ liệu bằng CTDL: ";
                SetConsoleTextAttribute(hConsole, BRIGHT_PURPLE_MAIN);
                cout << dsName << endl;
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                
                cout << "\tĐang chuyển đổi dữ liệu sang cấu trúc " << dsName << "..." << endl;
                if (studentList) { 
                    studentList->transferFromVector(students);
                    cout << "\tChuyển đổi thành công!" << endl;
                }
                break;
            }
            case 3:
                addStudent(*studentList);
                // Update the vector to keep data in sync
                students = studentList->getAllStudents();

                // Case Add data set Sorted = false 
                dataSorted = isSorted::NONE;
                break;
            case 4:
                updateStudent(*studentList);
                // Update the vector to keep data in sync
                students = studentList->getAllStudents();

                // Case Update data set Sorted = false 
                dataSorted = isSorted::NONE;
                break;
            case 5:
                deleteStudent(*studentList);
                // Update the vector to keep data in sync
                students = studentList->getAllStudents();
                break;            
            case 6: {
                int statChoice;
                do {
                    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
                    cout << "\n--- Thống Kê Sinh Viên ---\n";
                    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
                    cout << "\nHãy chọn một trong các tùy chọn sau:\n\n";
                    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                    cout << "\t1. Liệt kê các sinh viên có điểm thấp nhất\n";
                    cout << "\t2. Liệt kê các sinh viên có điểm cao nhất\n";
                    cout << "\t3. Tính điểm trung bình của lớp.\n";
                    cout << "\t0. Quay lại menu chính\n";
                    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE_MAIN);
                    cout << "\nLựa chọn của bạn (0-3): ";
                    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);

                    if (!(cin >> statChoice)) {
                        SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                        cout << "\tLựa chọn không hợp lệ. Vui lòng nhập số.\n";
                        SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);                    
                        cin.clear();
                        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                        continue;
                    }                
                    cin.ignore();
                    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
                    switch (statChoice) {
                        case 0:
                            // Return to main menu
                            break;
                        case 1:
                            cout << "\nLiệt kê các sinh viên có điểm thấp nhất:\n\n";
                            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN); 
                            listLowestScoringStudents(*studentList);
                            break;
                        case 2:
                            cout << "\nLiệt kê các sinh viên có điểm cao nhất:\n\n"; 
                            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);                        
                            listHighestScoringStudents(*studentList);
                            break;
                        case 3:
                            cout << "\nTính điểm trung bình của mỗi lớp:\n\n";
                            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                            calculateAverageScore(*studentList);
                            break;
                        default:
                            SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                            cout << "\nLựa chọn không hợp lệ.\n";
                            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                            break;
                    }
                    
                    if (statChoice != 0) {
                        cout << "\nNhấn Enter để tiếp tục...";
                        cin.get();
                    }
                } while (statChoice != 0);
                break;
            }
            case 7:
                sortStudents(*studentList, dataSorted);
                // Update the vector to keep data in sync (uncomment when sort is implemented)
                // students = studentList->getAllStudents();
                break;            
            case 8:
                searchStudents(*studentList, dataSorted, dataStructureType);
                break;
            case 0:
                SetConsoleTextAttribute(hConsole, BRIGHT_GREEN_MAIN);
                cout << "Thoát chương trình. Tạm biệt!\n";
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
                break;
            default:
                SetConsoleTextAttribute(hConsole, BRIGHT_RED_MAIN);
                cout << "Lựa chọn không hợp lệ, vui lòng thử lại!\n";
                SetConsoleTextAttribute(hConsole, DEFAULT_COLOR_MAIN);
        }
        
        if (choice != 0) {
            cout << "\nNhấn Enter để tiếp tục...";
            cin.get();
        }
        
    } while (choice != 0);

    // Clean up allocated memory
    if (studentList != nullptr) {
        students = studentList->getAllStudents(); 
        
        delete studentList;
        studentList = nullptr;
    }

    return 0;
}