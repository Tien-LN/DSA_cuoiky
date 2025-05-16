#pragma once
#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <vector> 
#include "enums.hpp" 

struct Student {
    std::string mssv;
    std::string ho;
    std::string ten;
    std::string lop;
    float diem;

    Student() = default; 

    Student(const std::string& id, const std::string& lastName, const std::string& firstName, const std::string& className, float score)
        : mssv(id), ho(lastName), ten(firstName), lop(className), diem(score) {
    }

    void print(SearchCriterionType criterion, const std::string& searchTerm) const;
    void print() const; 

    std::string getFullName() const {
        return ho + " " + ten;
    };

    std::string reversedName() const;
};

#endif 

