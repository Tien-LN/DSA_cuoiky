#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include "student.hpp"
#include "IStudentList.hpp" 
#include <vector> 

// Các hàm quản lý sinh viên 
void addStudent(IStudentList& studentList);

void updateStudent(IStudentList& studentList);

void deleteStudent(IStudentList& studentList);

void listLowestScoringStudents(const IStudentList& studentList);

void listHighestScoringStudents(const IStudentList& studentList);

double calculateAverageScore(const IStudentList& studentList);

void sortStudents(IStudentList& studentList);

void searchStudents(IStudentList& studentList);

void displayAllStudents(const IStudentList& studentList);

#endif