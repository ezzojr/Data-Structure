#include "Job.hpp"
#include <iostream>

// Default constructor
Job::Job() : id(0), title(""), company(""), description(""), 
             skillCount(0), experienceRequired(0) {}

// Parameterized constructor
Job::Job(int id, std::string title, std::string company, 
         std::string description, int experienceRequired)
    : id(id), title(title), company(company), description(description),
      skillCount(0), experienceRequired(experienceRequired) {}

// Getters
int Job::getId() const { return id; }
std::string Job::getTitle() const { return title; }
std::string Job::getCompany() const { return company; }
std::string Job::getDescription() const { return description; }
int Job::getExperienceRequired() const { return experienceRequired; }
int Job::getSkillCount() const { return skillCount; }

std::string Job::getSkill(int index) const {
    if (index >= 0 && index < skillCount) {
        return requiredSkills[index];
    }
    return "";
}

// Setters
void Job::setId(int id) { this->id = id; }
void Job::setTitle(std::string title) { this->title = title; }
void Job::setCompany(std::string company) { this->company = company; }
void Job::setDescription(std::string description) { this->description = description; }
void Job::setExperienceRequired(int years) { this->experienceRequired = years; }

// Add a skill (max 10)
bool Job::addSkill(std::string skill) {
    if (skillCount < 10) {
        requiredSkills[skillCount] = skill;
        skillCount++;
        return true;
    }
    return false;
}

// Check if job requires a specific skill
bool Job::hasSkill(std::string skill) const {
    for (int i = 0; i < skillCount; i++) {
        if (requiredSkills[i] == skill) {
            return true;
        }
    }
    return false;
}

// Display basic info
void Job::display() const {
    std::cout << "Job ID: " << id << std::endl;
    std::cout << "Title: " << title << std::endl;
    std::cout << "Company: " << company << std::endl;
}

// Display detailed info
void Job::displayDetailed() const {
    std::cout << "================================" << std::endl;
    std::cout << "Job ID: " << id << std::endl;
    std::cout << "Title: " << title << std::endl;
    std::cout << "Company: " << company << std::endl;
    std::cout << "Description: " << description << std::endl;
    std::cout << "Experience Required: " << experienceRequired << " years" << std::endl;
    std::cout << "Required Skills: ";
    for (int i = 0; i < skillCount; i++) {
        std::cout << requiredSkills[i];
        if (i < skillCount - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
}