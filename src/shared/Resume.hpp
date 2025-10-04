#ifndef RESUME_HPP
#define RESUME_HPP

#include <string>

class Resume {
private:
    int id;
    std::string name;
    std::string email;
    std::string summary;
    std::string skills[20];  // Max 20 skills
    int skillCount;
    int yearsOfExperience;

public:
    // Constructors
    Resume();
    Resume(int id, std::string name, std::string email, 
           std::string summary, int yearsOfExperience);
    
    // Getters
    int getId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getSummary() const;
    int getYearsOfExperience() const;
    int getSkillCount() const;
    std::string getSkill(int index) const;
    
    // Setters
    void setId(int id);
    void setName(std::string name);
    void setEmail(std::string email);
    void setSummary(std::string summary);
    void setYearsOfExperience(int years);
    
    // Skill management
    bool addSkill(std::string skill);
    bool hasSkill(std::string skill) const;
    
    // Display
    void display() const;
    void displayDetailed() const;
};

#endif