#ifndef MATCH_HPP
#define MATCH_HPP

#include "Job.hpp"
#include "Resume.hpp"

class Match {
private:
    int jobId;
    int resumeId;
    double score;  // Match score (0-100)
    int matchingSkills;
    
public:
    Match();
    Match(int jobId, int resumeId, double score, int matchingSkills);
    
    // Getters
    int getJobId() const;
    int getResumeId() const;
    double getScore() const;
    int getMatchingSkills() const;
    
    // Display
    void display() const;
};

#endif