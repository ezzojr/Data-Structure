#include "JobLinkedList.hpp"
#include <iostream>

// Constructor
JobLinkedList::JobLinkedList() : head(nullptr), tail(nullptr), size(0) {}

// Destructor
JobLinkedList::~JobLinkedList() {
    clear();
}

// Insert at end
void JobLinkedList::insert(const Job& job) {
    JobNode* newNode = new JobNode(job);
    
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

// Insert at beginning
void JobLinkedList::insertAtBeginning(const Job& job) {
    JobNode* newNode = new JobNode(job);
    
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

// Insert at specific position
void JobLinkedList::insertAtPosition(const Job& job, int position) {
    if (position < 0 || position > size) {
        std::cout << "Invalid position!" << std::endl;
        return;
    }
    
    if (position == 0) {
        insertAtBeginning(job);
        return;
    }
    
    if (position == size) {
        insert(job);
        return;
    }
    
    JobNode* newNode = new JobNode(job);
    JobNode* current = head;
    
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    newNode->next = current->next;
    current->next = newNode;
    size++;
}

// Remove by job ID
bool JobLinkedList::remove(int jobId) {
    if (isEmpty()) return false;
    
    if (head->data.getId() == jobId) {
        JobNode* temp = head;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        delete temp;
        size--;
        return true;
    }
    
    JobNode* current = head;
    while (current->next != nullptr && current->next->data.getId() != jobId) {
        current = current->next;
    }
    
    if (current->next == nullptr) return false;
    
    JobNode* temp = current->next;
    current->next = current->next->next;
    
    if (temp == tail) tail = current;
    
    delete temp;
    size--;
    return true;
}

// Clear all nodes
void JobLinkedList::clear() {
    JobNode* current = head;
    while (current != nullptr) {
        JobNode* temp = current;
        current = current->next;
        delete temp;
    }
    head = tail = nullptr;
    size = 0;
}

// Search by ID
Job* JobLinkedList::search(int jobId) {
    JobNode* current = head;
    while (current != nullptr) {
        if (current->data.getId() == jobId) {
            return &(current->data);
        }
        current = current->next;
    }
    return nullptr;
}

// Search by title
Job* JobLinkedList::searchByTitle(const std::string& title) {
    JobNode* current = head;
    while (current != nullptr) {
        if (current->data.getTitle() == title) {
            return &(current->data);
        }
        current = current->next;
    }
    return nullptr;
}

// Get head pointer
JobNode* JobLinkedList::getHead() const {
    return head;
}

// Get list size
int JobLinkedList::getSize() const {
    return size;
}

// Check if empty
bool JobLinkedList::isEmpty() const {
    return head == nullptr;
}

// Display all jobs
void JobLinkedList::display() const {
    if (isEmpty()) {
        std::cout << "No jobs in the list." << std::endl;
        return;
    }
    
    std::cout << "\n========== JOB LIST ==========" << std::endl;
    JobNode* current = head;
    int count = 1;
    
    while (current != nullptr) {
        std::cout << "\n[" << count++ << "] ";
        current->data.display();
        current = current->next;
    }
    std::cout << "\nTotal Jobs: " << size << std::endl;
    std::cout << "==============================\n" << std::endl;
}

// Display detailed info
void JobLinkedList::displayDetailed() const {
    if (isEmpty()) {
        std::cout << "No jobs in the list." << std::endl;
        return;
    }
    
    std::cout << "\n========== DETAILED JOB LIST ==========" << std::endl;
    JobNode* current = head;
    
    while (current != nullptr) {
        current->data.displayDetailed();
        std::cout << std::endl;
        current = current->next;
    }
    std::cout << "Total Jobs: " << size << std::endl;
    std::cout << "=======================================\n" << std::endl;
}

// Sort by ID (Bubble Sort)
void JobLinkedList::sortById() {
    if (size < 2) return;
    
    bool swapped;
    do {
        swapped = false;
        JobNode* current = head;
        
        while (current->next != nullptr) {
            if (current->data.getId() > current->next->data.getId()) {
                swapNodes(current, current->next);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

// Sort by experience required (Bubble Sort)
void JobLinkedList::sortByExperience() {
    if (size < 2) return;
    
    bool swapped;
    do {
        swapped = false;
        JobNode* current = head;
        
        while (current->next != nullptr) {
            if (current->data.getExperienceRequired() > current->next->data.getExperienceRequired()) {
                swapNodes(current, current->next);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

// Swap node data
void JobLinkedList::swapNodes(JobNode* a, JobNode* b) {
    Job temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// Get job at index
Job* JobLinkedList::getJobAt(int index) {
    if (index < 0 || index >= size) return nullptr;
    
    JobNode* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return &(current->data);
}
