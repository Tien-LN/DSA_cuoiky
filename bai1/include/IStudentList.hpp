#ifndef ISTUDENT_LIST_HPP
#define ISTUDENT_LIST_HPP

#include <vector>
#include <string>
#include "student.hpp"
#include "types.hpp" 

using namespace std;

class IStudentList {
public:
    virtual ~IStudentList() = default;

    virtual bool add(const Student& student) = 0;

    virtual bool update(const string& studentId, const Student& updatedStudent) = 0;

    virtual bool remove(const string& studentId) = 0;

    virtual bool listLowestScoringStudents() const = 0;

    virtual bool listHighestScoringStudents() const = 0;
    
    virtual double calculateAverageScore() const = 0;

    virtual double sort(SortAlgorithmType algorithm, SortCriterionType criterion) = 0;

    virtual SearchResult search(SearchCriterionType criterion, const string& searchTerm, bool reverseName) const = 0;

    virtual vector<Student> getAllStudents() const = 0;

    virtual void transferFromVector(const vector<Student>& students) = 0;
};

#endif
