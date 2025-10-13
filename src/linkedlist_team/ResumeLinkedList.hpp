#ifndef RESUMELINKEDLIST_HPP
#define RESUMELINKEDLIST_HPP

#include "../shared/Resume.hpp"

// Node structure for Resume Linked List
struct ResumeNode {
    Resume data;
    ResumeNode* next;
    
    // Constructor
    ResumeNode(const Resume& resume) : data(resume), next(nullptr) {}
};

class ResumeLinkedList {
private:
    ResumeNode* head;
    ResumeNode* tail;
    int size;

public:
    // Constructor & Destructor
    ResumeLinkedList();
    ~ResumeLinkedList();
    
    // Basic operations
    void insert(const Resume& resume);
    void insertAtBeginning(const Resume& resume);
    void insertAtPosition(const Resume& resume, int position);
    bool remove(int resumeId);
    void clear();
    
    // Search operations
    Resume* search(int resumeId);
    Resume* searchByName(const std::string& name);
    ResumeNode* getHead() const;
    
    // Utility operations
    int getSize() const;
    bool isEmpty() const;
    void display() const;
    void displayDetailed() const;
    
    // Sorting operations
    void sortById();
    void sortByExperience();
    
    // Get resume at index
    Resume* getResumeAt(int index);
    
private:
    // Helper function for sorting
    void swapNodes(ResumeNode* a, ResumeNode* b);
};

#endif
