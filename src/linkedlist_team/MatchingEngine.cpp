#include "MatchingEngine.hpp"
#include <iostream>
#include <iomanip>

// Calculate overall match score
double MatchingEngine::calculateMatchScore(const Job& job, const Resume& resume) {
    // Skill matching score (0-100)
    int matchingSkills = countMatchingSkills(job, resume);
    int totalJobSkills = job.getSkillCount();
    
    double skillScore = 0.0;
    if (totalJobSkills > 0) {
        skillScore = (static_cast<double>(matchingSkills) / totalJobSkills) * 100.0;
    }
    
    // Experience score (0-100)
    double experienceScore = calculateExperienceScore(job, resume);
    
    // Weighted final score
    double finalScore = (skillScore * SKILL_WEIGHT / 100.0) + 
                        (experienceScore * EXPERIENCE_WEIGHT / 100.0);
    
    return finalScore;
}

// Count matching skills between job and resume
int MatchingEngine::countMatchingSkills(const Job& job, const Resume& resume) {
    int count = 0;
    
    for (int i = 0; i < job.getSkillCount(); i++) {
        std::string jobSkill = job.getSkill(i);
        
        for (int j = 0; j < resume.getSkillCount(); j++) {
            std::string resumeSkill = resume.getSkill(j);
            
            if (jobSkill == resumeSkill) {
                count++;
                break;
            }
        }
    }
    
    return count;
}

// Check if resume meets experience requirement
bool MatchingEngine::meetsExperienceRequirement(const Job& job, const Resume& resume) {
    return resume.getYearsOfExperience() >= job.getExperienceRequired();
}

// Calculate experience score (0-100)
double MatchingEngine::calculateExperienceScore(const Job& job, const Resume& resume) {
    int required = job.getExperienceRequired();
    int actual = resume.getYearsOfExperience();
    
    if (required == 0) {
        return 100.0;
    }
    
    if (actual >= required) {
        double ratio = static_cast<double>(actual) / required;
        return (ratio >= 2.0) ? 100.0 : 50.0 + (ratio * 50.0);
    } else {
        return (static_cast<double>(actual) / required) * 100.0;
    }
}

// Display detailed match information
void MatchingEngine::displayMatchDetails(const Job& job, const Resume& resume, double score) {
    std::cout << "\n============================================================" << std::endl;
    std::cout << "              DETAILED MATCH ANALYSIS                       " << std::endl;
    std::cout << "============================================================" << std::endl;
    
    std::cout << "\nJOB DETAILS:" << std::endl;
    std::cout << "   ID: " << job.getId() << std::endl;
    std::cout << "   Title: " << job.getTitle() << std::endl;
    std::cout << "   Company: " << job.getCompany() << std::endl;
    std::cout << "   Experience Required: " << job.getExperienceRequired() << " years" << std::endl;
    
    std::cout << "\nCANDIDATE DETAILS:" << std::endl;
    std::cout << "   ID: " << resume.getId() << std::endl;
    std::cout << "   Name: " << resume.getName() << std::endl;
    std::cout << "   Experience: " << resume.getYearsOfExperience() << " years" << std::endl;
    
    int matchingSkills = countMatchingSkills(job, resume);
    std::cout << "\nSKILL MATCHING:" << std::endl;
    std::cout << "   Required Skills: " << job.getSkillCount() << std::endl;
    std::cout << "   Matching Skills: " << matchingSkills << std::endl;
    std::cout << "   Skills Match Rate: " << std::fixed << std::setprecision(1) 
              << (job.getSkillCount() > 0 ? (matchingSkills * 100.0 / job.getSkillCount()) : 0) 
              << "%" << std::endl;
    
    if (matchingSkills > 0) {
        std::cout << "\n   Matched Skills: ";
        bool first = true;
        for (int i = 0; i < job.getSkillCount(); i++) {
            std::string jobSkill = job.getSkill(i);
            if (resume.hasSkill(jobSkill)) {
                if (!first) std::cout << ", ";
                std::cout << jobSkill;
                first = false;
            }
        }
        std::cout << std::endl;
    }
    
    double expScore = calculateExperienceScore(job, resume);
    std::cout << "\nEXPERIENCE ANALYSIS:" << std::endl;
    std::cout << "   Experience Score: " << std::fixed << std::setprecision(1) 
              << expScore << "/100" << std::endl;
    std::cout << "   Requirement Status: " 
              << (meetsExperienceRequirement(job, resume) ? "[MEETS]" : "[BELOW]") 
              << std::endl;
    
    std::cout << "\nOVERALL MATCH SCORE: " << std::fixed << std::setprecision(2) 
              << score << "/100" << std::endl;
    
    std::cout << "\nRECOMMENDATION: ";
    if (score >= 80) {
        std::cout << "EXCELLENT MATCH - Highly Recommended" << std::endl;
    } else if (score >= 60) {
        std::cout << "GOOD MATCH - Recommended" << std::endl;
    } else if (score >= 40) {
        std::cout << "MODERATE MATCH - Consider for Interview" << std::endl;
    } else {
        std::cout << "LOW MATCH - Not Recommended" << std::endl;
    }
    
    std::cout << "\n------------------------------------------------------------\n" << std::endl;
}