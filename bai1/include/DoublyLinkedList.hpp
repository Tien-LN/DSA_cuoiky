#ifndef STUDENT_DOUBLY_LINKED_LIST_HPP
#define STUDENT_DOUBLY_LINKED_LIST_HPP

#include "IStudentList.hpp"
#include "student.hpp"
#include "utils.hpp"
#include <string>
#include <stdexcept>

class DoublyLinkedList : public IStudentList {
private:
    struct Node {
        Student data;
        Node* next;
        Node* prev;
        
        Node(const Student& student) : data(student), next(nullptr), prev(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    int size;

public:
    DoublyLinkedList();
    ~DoublyLinkedList() override;
    
    bool add(const Student& student) override;
    bool update(const std::string& studentId, const Student& updatedStudent) override;
    bool remove(const std::string& studentId) override;
    bool listLowestScoringStudents() const override;
    bool listHighestScoringStudents() const override;
    double calculateAverageScore() const override;
    double sort(SortAlgorithmType algorithm, SortCriterionType criterion) override;
    std::string getFieldByCriterion(const Student &student, SearchCriterionType criterion) const;
    SearchResult search(SearchCriterionType criterion, const std::string& searchTerm, bool reverseName,int searchAlgoChoice) const override;
    std::vector<Student> getAllStudents() const override;
    void transferFromVector(const std::vector<Student>& initialStudents) override;
};

#endif