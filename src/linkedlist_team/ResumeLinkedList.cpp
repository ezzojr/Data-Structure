#include "ResumeLinkedList.hpp"
#include <iostream>

// Constructor
ResumeLinkedList::ResumeLinkedList() : head(nullptr), tail(nullptr), size(0) {}

// Destructor
ResumeLinkedList::~ResumeLinkedList() {
    clear();
}

// Insert at end
void ResumeLinkedList::insert(const Resume& resume) {
    ResumeNode* newNode = new ResumeNode(resume);
    
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

// Insert at beginning
void ResumeLinkedList::insertAtBeginning(const Resume& resume) {
    ResumeNode* newNode = new ResumeNode(resume);
    
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

// Insert at specific position
void ResumeLinkedList::insertAtPosition(const Resume& resume, int position) {
    if (position < 0 || position > size) {
        std::cout << "Invalid position!" << std::endl;
        return;
    }
    
    if (position == 0) {
        insertAtBeginning(resume);
        return;
    }
    
    if (position == size) {
        insert(resume);
        return;
    }
    
    ResumeNode* newNode = new ResumeNode(resume);
    ResumeNode* current = head;
    
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    newNode->next = current->next;
    current->next = newNode;
    size++;
}

// Remove by resume ID
bool ResumeLinkedList::remove(int resumeId) {
    if (isEmpty()) return false;
    
    if (head->data.getId() == resumeId) {
        ResumeNode* temp = head;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        delete temp;
        size--;
        return true;
    }
    
    ResumeNode* current = head;
    while (current->next != nullptr && current->next->data.getId() != resumeId) {
        current = current->next;
    }
    
    if (current->next == nullptr) return false;
    
    ResumeNode* temp = current->next;
    current->next = current->next->next;
    
    if (temp == tail) tail = current;
    
    delete temp;
    size--;
    return true;
}

// Clear all nodes
void ResumeLinkedList::clear() {
    ResumeNode* current = head;
    while (current != nullptr) {
        ResumeNode* temp = current;
        current = current->next;
        delete temp;
    }
    head = tail = nullptr;
    size = 0;
}

// Search by ID
Resume* ResumeLinkedList::search(int resumeId) {
    ResumeNode* current = head;
    while (current != nullptr) {
        if (current->data.getId() == resumeId) {
            return &(current->data);
        }
        current = current->next;
    }
    return nullptr;
}

// Search by name
Resume* ResumeLinkedList::searchByName(const std::string& name) {
    ResumeNode* current = head;
    while (current != nullptr) {
        if (current->data.getName() == name) {
            return &(current->data);
        }
        current = current->next;
    }
    return nullptr;
}

// Get head pointer
ResumeNode* ResumeLinkedList::getHead() const {
    return head;
}

// Get list size
int ResumeLinkedList::getSize() const {
    return size;
}

// Check if empty
bool ResumeLinkedList::isEmpty() const {
    return head == nullptr;
}

// Display all resumes
void ResumeLinkedList::display() const {
    if (isEmpty()) {
        std::cout << "No resumes in the list." << std::endl;
        return;
    }
    
    std::cout << "\n========== RESUME LIST ==========" << std::endl;
    ResumeNode* current = head;
    int count = 1;
    
    while (current != nullptr) {
        std::cout << "\n[" << count++ << "] ";
        current->data.display();
        current = current->next;
    }
    std::cout << "\nTotal Resumes: " << size << std::endl;
    std::cout << "=================================\n" << std::endl;
}

// Display detailed info
void ResumeLinkedList::displayDetailed() const {
    if (isEmpty()) {
        std::cout << "No resumes in the list." << std::endl;
        return;
    }
    
    std::cout << "\n========== DETAILED RESUME LIST ==========" << std::endl;
    ResumeNode* current = head;
    
    while (current != nullptr) {
        current->data.displayDetailed();
        std::cout << std::endl;
        current = current->next;
    }
    std::cout << "Total Resumes: " << size << std::endl;
    std::cout << "==========================================\n" << std::endl;
}

// Sort by ID (Bubble Sort)
void ResumeLinkedList::sortById() {
    if (size < 2) return;
    
    bool swapped;
    do {
        swapped = false;
        ResumeNode* current = head;
        
        while (current->next != nullptr) {
            if (current->data.getId() > current->next->data.getId()) {
                swapNodes(current, current->next);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

// Sort by experience (Bubble Sort)
void ResumeLinkedList::sortByExperience() {
    if (size < 2) return;
    
    bool swapped;
    do {
        swapped = false;
        ResumeNode* current = head;
        
        while (current->next != nullptr) {
            if (current->data.getYearsOfExperience() > current->next->data.getYearsOfExperience()) {
                swapNodes(current, current->next);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

// Swap node data
void ResumeLinkedList::swapNodes(ResumeNode* a, ResumeNode* b) {
    Resume temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// Get resume at index
Resume* ResumeLinkedList::getResumeAt(int index) {
    if (index < 0 || index >= size) return nullptr;
    
    ResumeNode* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return &(current->data);
}
