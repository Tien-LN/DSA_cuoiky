#ifndef STUDENT_ARRAY_HPP
#define STUDENT_ARRAY_HPP

#include "IStudentList.hpp"
#include "student.hpp"
#include <string>
#include <stdexcept>

const int MAX_STUDENTS = 100; 

class StudentArray : public IStudentList {
private:
    Student students[MAX_STUDENTS];
    int currentSize;

public:
    StudentArray();
    ~StudentArray() override = default;    bool add(const Student& student) override;
    bool update(const std::string& studentId, const Student& updatedStudent) override;
    bool remove(const std::string& studentId) override;
    bool listLowestScoringStudents() const override;
    bool listHighestScoringStudents() const override;
    double calculateAverageScore() const override;
    double sort(SortAlgorithmType algorithm, SortCriterionType criterion) override;
    SearchResult search(SearchCriterionType criterion, const std::string& searchTerm, bool reverseName) const override;
    std::vector<Student> getAllStudents() const override;
    void transferFromVector(const std::vector<Student>& initialStudents) override;
};

#endif