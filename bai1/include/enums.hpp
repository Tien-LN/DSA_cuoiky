#ifndef ENUMS_HPP
#define ENUMS_HPP

// Sorting algorithms
enum class SortAlgorithmType {
    BUBBLE,
    INSERTION,
    QUICK,
    SELECTION,
    HEAP,
    MERGE,
    NONE 
};

// Sorting criteria
enum class SortCriterionType {
    MSSV,      
    HO,
    TEN,  
    DIEM       
};

// Searching criteria
enum class SearchCriterionType {
    MSSV,
    HO,         
    TEN,        
    LOP,        
    DIEM
};

// Data structure types
enum class DataStructureType {
    ARRAY,
    SINGLY_LINKED_LIST,
    DOUBLY_LINKED_LIST,
    CIRCULAR_LINKED_LIST
};


// Check if Sorted 
enum isSorted {
    NONE,
    MSSV,
    HO,
    TEN,
    LOP,
    DIEM
};
#endif 