#ifndef MATCHINGENGINE_HPP
#define MATCHINGENGINE_HPP

#include "../shared/Job.hpp"
#include "../shared/Resume.hpp"
#include "../shared/Match.hpp"

// Simple array-based structure to store matches
struct MatchArray {
    Match* matches;
    int size;
    int capacity;
    
    MatchArray(int cap = 100) : size(0), capacity(cap) {
        matches = new Match[capacity];
    }
    
    ~MatchArray() {
        delete[] matches;
    }
    
    void add(const Match& match) {
        if (size < capacity) {
            matches[size++] = match;
        }
    }
    
    void clear() {
        size = 0;
    }
};

class MatchingEngine {
private:
    // Weight factors for scoring (total = 100%)
    static const int SKILL_WEIGHT = 60;
    static const int EXPERIENCE_WEIGHT = 40;
    
public:
    // Calculate match score between a job and resume
    static double calculateMatchScore(const Job& job, const Resume& resume);
    
    // Count matching skills
    static int countMatchingSkills(const Job& job, const Resume& resume);
    
    // Check if experience requirement is met
    static bool meetsExperienceRequirement(const Job& job, const Resume& resume);
    
    // Calculate experience score (0-100)
    static double calculateExperienceScore(const Job& job, const Resume& resume);
    
    // Display match details
    static void displayMatchDetails(const Job& job, const Resume& resume, double score);
};

#endif
