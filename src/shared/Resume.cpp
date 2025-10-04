#include "Resume.hpp"
#include <iostream>

// Default constructor
Resume::Resume() : id(0), name(""), email(""), summary(""), 
                   skillCount(0), yearsOfExperience(0) {}

// Parameterized constructor
Resume::Resume(int id, std::string name, std::string email, 
               std::string summary, int yearsOfExperience)
    : id(id), name(name), email(email), summary(summary),
      skillCount(0), yearsOfExperience(yearsOfExperience) {}

// Getters
int Resume::getId() const { return id; }
std::string Resume::getName() const { return name; }
std::string Resume::getEmail() const { return email; }
std::string Resume::getSummary() const { return summary; }
int Resume::getYearsOfExperience() const { return yearsOfExperience; }
int Resume::getSkillCount() const { return skillCount; }

std::string Resume::getSkill(int index) const {
    if (index >= 0 && index < skillCount) {
        return skills[index];
    }
    return "";
}

// Setters
void Resume::setId(int id) { this->id = id; }
void Resume::setName(std::string name) { this->name = name; }
void Resume::setEmail(std::string email) { this->email = email; }
void Resume::setSummary(std::string summary) { this->summary = summary; }
void Resume::setYearsOfExperience(int years) { this->yearsOfExperience = years; }

// Add a skill (max 20)
bool Resume::addSkill(std::string skill) {
    if (skillCount < 20) {
        skills[skillCount] = skill;
        skillCount++;
        return true;
    }
    return false;
}

// Check if candidate has a specific skill
bool Resume::hasSkill(std::string skill) const {
    for (int i = 0; i < skillCount; i++) {
        if (skills[i] == skill) {
            return true;
        }
    }
    return false;
}

// Display basic info
void Resume::display() const {
    std::cout << "Resume ID: " << id << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Email: " << email << std::endl;
}

// Display detailed info
void Resume::displayDetailed() const {
    std::cout << "================================" << std::endl;
    std::cout << "Resume ID: " << id << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Summary: " << summary << std::endl;
    std::cout << "Years of Experience: " << yearsOfExperience << std::endl;
    std::cout << "Skills: ";
    for (int i = 0; i < skillCount; i++) {
        std::cout << skills[i];
        if (i < skillCount - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
}