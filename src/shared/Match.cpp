#include "Match.hpp"
#include <iostream>

Match::Match() : jobId(0), resumeId(0), score(0.0), matchingSkills(0) {}

Match::Match(int jobId, int resumeId, double score, int matchingSkills)
    : jobId(jobId), resumeId(resumeId), score(score), matchingSkills(matchingSkills) {}

int Match::getJobId() const { return jobId; }
int Match::getResumeId() const { return resumeId; }
double Match::getScore() const { return score; }
int Match::getMatchingSkills() const { return matchingSkills; }

void Match::display() const {
    std::cout << "Job ID: " << jobId 
              << " | Resume ID: " << resumeId 
              << " | Score: " << score << "%" 
              << " | Matching Skills: " << matchingSkills 
              << std::endl;
}