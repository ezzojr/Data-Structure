#ifndef JOB_HPP
#define JOB_HPP

#include <string>

class Job {
private:
    int id;
    std::string title;
    std::string company;
    std::string description;
    std::string requiredSkills[10];  // Max 10 skills
    int skillCount;
    int experienceRequired;  // Years of experience

public:
    // Constructors
    Job();
    Job(int id, std::string title, std::string company, 
        std::string description, int experienceRequired);
    
    // Getters
    int getId() const;
    std::string getTitle() const;
    std::string getCompany() const;
    std::string getDescription() const;
    int getExperienceRequired() const;
    int getSkillCount() const;
    std::string getSkill(int index) const;
    
    // Setters
    void setId(int id);
    void setTitle(std::string title);
    void setCompany(std::string company);
    void setDescription(std::string description);
    void setExperienceRequired(int years);
    
    // Skill management
    bool addSkill(std::string skill);
    bool hasSkill(std::string skill) const;
    
    // Display
    void display() const;
    void displayDetailed() const;
};

#endif