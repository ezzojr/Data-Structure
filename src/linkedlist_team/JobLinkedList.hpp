#ifndef JOBLINKEDLIST_HPP
#define JOBLINKEDLIST_HPP

#include "../shared/Job.hpp"

// Node structure for Job Linked List
struct JobNode {
    Job data;
    JobNode* next;
    
    // Constructor
    JobNode(const Job& job) : data(job), next(nullptr) {}
};

class JobLinkedList {
private:
    JobNode* head;
    JobNode* tail;
    int size;

public:
    // Constructor & Destructor
    JobLinkedList();
    ~JobLinkedList();
    
    // Basic operations
    void insert(const Job& job);
    void insertAtBeginning(const Job& job);
    void insertAtPosition(const Job& job, int position);
    bool remove(int jobId);
    void clear();
    
    // Search operations
    Job* search(int jobId);
    Job* searchByTitle(const std::string& title);
    JobNode* getHead() const;
    
    // Utility operations
    int getSize() const;
    bool isEmpty() const;
    void display() const;
    void displayDetailed() const;
    
    // Sorting operations
    void sortById();
    void sortByExperience();
    
    // Get job at index
    Job* getJobAt(int index);
    
private:
    // Helper function for sorting
    void swapNodes(JobNode* a, JobNode* b);
};

#endif
