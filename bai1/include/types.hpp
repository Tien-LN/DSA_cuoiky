#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>
#include <string>
#include "student.hpp" 
#include "enums.hpp"   

struct SearchResult {
    std::vector<Student> students;
    double timeTakenMilliseconds;

    SearchResult() : students(), timeTakenMilliseconds(0.0) {}

    SearchResult(const std::vector<Student>& students, double timeTakenMilliseconds)
        : students(students), timeTakenMilliseconds(timeTakenMilliseconds) {}
};

#endif 
