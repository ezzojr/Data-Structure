#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

// Include Linked List implementation
#include "linkedlist_team/JobLinkedList.hpp"
#include "linkedlist_team/ResumeLinkedList.hpp"
#include "linkedlist_team/MatchingEngine.hpp"

#include "shared/CSVReader.hpp"

using namespace std;
using namespace chrono;

// Function prototypes
void runLinkedListImplementation();
void loadJobsFromCSV_LL(const char* filename, JobLinkedList& jobList);
void loadResumesFromCSV_LL(const char* filename, ResumeLinkedList& resumeList);
void extractSkills(const string& text, Job* job, Resume* resume);
void performMatching_LL(JobLinkedList& jobList, ResumeLinkedList& resumeList, MatchArray& matches);
void displayTopMatches_LL(const MatchArray& matches, int top, JobLinkedList& jobList, ResumeLinkedList& resumeList);
void displayMenu_LL();
void displayPerformanceMetrics_LL(long long loadTime, int dataSize);

// Common tech skills to look for
const string COMMON_SKILLS[] = {
    "C++", "Python", "Java", "JavaScript", "SQL", "HTML", "CSS",
    "React", "Node.js", "MongoDB", "MySQL", "PostgreSQL",
    "Git", "Docker", "Kubernetes", "AWS", "Azure",
    "Machine Learning", "Deep Learning", "NLP", "Pandas",
    "Excel", "Power BI", "Tableau", "Data Analysis",
    "Statistics", "TensorFlow", "PyTorch", "REST API",
    "Spring", "Django", "Flask", "Angular", "Vue.js"
};
const int SKILLS_COUNT = 33;

int main() {
    cout << "\n===============================================" << endl;
    cout << "    JOB MATCHING SYSTEM - DSTR PROJECT" << endl;
    cout << "         Asia Pacific University" << endl;
    cout << "===============================================\n" << endl;
    
    int choice;
    bool running = true;
    
    while (running) {
        cout << "\n===============================================" << endl;
        cout << "     SELECT DATA STRUCTURE IMPLEMENTATION" << endl;
        cout << "===============================================" << endl;
        cout << "  1. Linked List Implementation" << endl;
        cout << "  2. Array Implementation (Coming Soon)" << endl;
        cout << "  3. Compare Both Implementations" << endl;
        cout << "  0. Exit" << endl;
        cout << "===============================================" << endl;
        
        cout << "\nEnter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "\n>>> Running Linked List Implementation...\n" << endl;
                runLinkedListImplementation();
                break;
                
            case 2:
                cout << "\n>>> Array Implementation - Coming Soon!" << endl;
                cout << "This will be implemented by Team Member 1 & 2.\n" << endl;
                break;
                
            case 3:
                cout << "\n>>> Performance Comparison - Coming Soon!" << endl;
                cout << "This will compare both implementations.\n" << endl;
                break;
                
            case 0:
                cout << "\nThank you for using the Job Matching System!" << endl;
                running = false;
                break;
                
            default:
                cout << "\nInvalid choice! Please try again." << endl;
        }
    }
    
    return 0;
}

void runLinkedListImplementation() {
    cout << "===============================================" << endl;
    cout << "  JOB MATCHING - LINKED LIST IMPLEMENTATION" << endl;
    cout << "===============================================\n" << endl;
    
    JobLinkedList jobList;
    ResumeLinkedList resumeList;
    MatchArray matches(10000);
    
    auto startLoad = high_resolution_clock::now();
    
    cout << "Loading data from CSV files..." << endl;
    loadJobsFromCSV_LL("../data/job_description.csv", jobList);
    loadResumesFromCSV_LL("../data/resume.csv", resumeList);
    
    auto endLoad = high_resolution_clock::now();
    long long loadTime = duration_cast<microseconds>(endLoad - startLoad).count();
    
    cout << "Jobs loaded: " << jobList.getSize() << endl;
    cout << "Resumes loaded: " << resumeList.getSize() << endl;
    cout << "Load time: " << loadTime << " microseconds\n" << endl;
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMenu_LL();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                cout << "\nDisplaying all jobs..." << endl;
                jobList.display();
                break;
            }
            
            case 2: {
                cout << "\nDisplaying all resumes..." << endl;
                resumeList.display();
                break;
            }
            
            case 3: {
                int jobId;
                cout << "\nEnter Job ID to search: ";
                cin >> jobId;
                
                auto startSearch = high_resolution_clock::now();
                Job* job = jobList.search(jobId);
                auto endSearch = high_resolution_clock::now();
                long long searchTime = duration_cast<microseconds>(endSearch - startSearch).count();
                
                if (job != nullptr) {
                    cout << "\nJob found!" << endl;
                    job->displayDetailed();
                    cout << "Search time: " << searchTime << " microseconds" << endl;
                } else {
                    cout << "\nJob not found!" << endl;
                }
                break;
            }
            
            case 4: {
                int resumeId;
                cout << "\nEnter Resume ID to search: ";
                cin >> resumeId;
                
                auto startSearch = high_resolution_clock::now();
                Resume* resume = resumeList.search(resumeId);
                auto endSearch = high_resolution_clock::now();
                long long searchTime = duration_cast<microseconds>(endSearch - startSearch).count();
                
                if (resume != nullptr) {
                    cout << "\nResume found!" << endl;
                    resume->displayDetailed();
                    cout << "Search time: " << searchTime << " microseconds" << endl;
                } else {
                    cout << "\nResume not found!" << endl;
                }
                break;
            }
            
            case 5: {
                cout << "\nSorting jobs by ID..." << endl;
                auto startSort = high_resolution_clock::now();
                jobList.sortById();
                auto endSort = high_resolution_clock::now();
                long long sortTime = duration_cast<microseconds>(endSort - startSort).count();
                
                cout << "Jobs sorted successfully!" << endl;
                cout << "Sort time: " << sortTime << " microseconds" << endl;
                break;
            }
            
            case 6: {
                cout << "\nSorting resumes by experience..." << endl;
                auto startSort = high_resolution_clock::now();
                resumeList.sortByExperience();
                auto endSort = high_resolution_clock::now();
                long long sortTime = duration_cast<microseconds>(endSort - startSort).count();
                
                cout << "Resumes sorted successfully!" << endl;
                cout << "Sort time: " << sortTime << " microseconds" << endl;
                break;
            }
            
            case 7: {
                cout << "\nPerforming job matching analysis..." << endl;
                matches.clear();
                
                auto startMatch = high_resolution_clock::now();
                performMatching_LL(jobList, resumeList, matches);
                auto endMatch = high_resolution_clock::now();
                long long matchTime = duration_cast<microseconds>(endMatch - startMatch).count();
                
                cout << "Matching complete!" << endl;
                cout << "Total matches found: " << matches.size << endl;
                cout << "Matching time: " << matchTime << " microseconds\n" << endl;
                break;
            }
            
            case 8: {
                if (matches.size == 0) {
                    cout << "\nNo matches found! Please perform matching first (Option 7)." << endl;
                } else {
                    int top;
                    cout << "\nEnter number of top matches to display: ";
                    cin >> top;
                    displayTopMatches_LL(matches, top, jobList, resumeList);
                }
                break;
            }
            
            case 9: {
                int jobId;
                cout << "\nEnter Job ID to match: ";
                cin >> jobId;
                
                Job* job = jobList.search(jobId);
                if (job == nullptr) {
                    cout << "Job not found!" << endl;
                    break;
                }
                
                cout << "\nMatching job with all candidates..." << endl;
                job->displayDetailed();
                
                MatchArray specificMatches(resumeList.getSize());
                ResumeNode* current = resumeList.getHead();
                
                while (current != nullptr) {
                    double score = MatchingEngine::calculateMatchScore(*job, current->data);
                    int skillCount = MatchingEngine::countMatchingSkills(*job, current->data);
                    specificMatches.add(Match(job->getId(), current->data.getId(), score, skillCount));
                    current = current->next;
                }
                
                displayTopMatches_LL(specificMatches, 10, jobList, resumeList);
                break;
            }
            
            case 10: {
                int resumeId;
                cout << "\nEnter Resume ID to match: ";
                cin >> resumeId;
                
                Resume* resume = resumeList.search(resumeId);
                if (resume == nullptr) {
                    cout << "Resume not found!" << endl;
                    break;
                }
                
                cout << "\nMatching candidate with all jobs..." << endl;
                resume->displayDetailed();
                
                MatchArray specificMatches(jobList.getSize());
                JobNode* current = jobList.getHead();
                
                while (current != nullptr) {
                    double score = MatchingEngine::calculateMatchScore(current->data, *resume);
                    int skillCount = MatchingEngine::countMatchingSkills(current->data, *resume);
                    specificMatches.add(Match(current->data.getId(), resume->getId(), score, skillCount));
                    current = current->next;
                }
                
                displayTopMatches_LL(specificMatches, 10, jobList, resumeList);
                break;
            }
            
            case 11: {
                cout << "\nPerformance Metrics - Linked List" << endl;
                cout << "===============================================" << endl;
                displayPerformanceMetrics_LL(loadTime, jobList.getSize() + resumeList.getSize());
                break;
            }
            
            case 0: {
                cout << "\nReturning to main menu..." << endl;
                running = false;
                break;
            }
            
            default:
                cout << "\nInvalid choice! Please try again." << endl;
        }
        
        if (running) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
}

void loadJobsFromCSV_LL(const char* filename, JobLinkedList& jobList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        return;
    }
    
    string line;
    // Skip header row
    getline(file, line);
    
    int id = 1;
    int loaded = 0;
    
    while (getline(file, line) && loaded < 100) {
        if (line.empty() || line.length() < 20) continue;
        
        // Remove quotes if present
        if (line.front() == '"') line = line.substr(1);
        if (line.back() == '"') line.pop_back();
        
        // Extract job title from description
        string title = "Position";
        size_t neededPos = line.find(" needed");
        size_t requiredPos = line.find(" required");
        
        if (neededPos != string::npos && neededPos > 0) {
            title = line.substr(0, neededPos);
        } else if (requiredPos != string::npos && requiredPos > 0) {
            title = line.substr(0, requiredPos);
        }
        
        // Create job with actual data
        Job job(id, title, "Tech Company", line, 3);
        extractSkills(line, &job, nullptr);
        
        jobList.insert(job);
        id++;
        loaded++;
    }
    
    file.close();
}

void loadResumesFromCSV_LL(const char* filename, ResumeLinkedList& resumeList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        return;
    }
    
    string line;
    // Skip header row
    getline(file, line);
    
    int id = 101;
    int loaded = 0;
    
    while (getline(file, line) && loaded < 100) {
        if (line.empty() || line.length() < 20) continue;
        
        // Remove quotes if present
        if (line.front() == '"') line = line.substr(1);
        if (line.back() == '"') line.pop_back();
        
        // Generate unique candidate name
        stringstream nameStream;
        nameStream << "Candidate_" << id;
        string candidateName = nameStream.str();
        
        // Generate unique email
        stringstream emailStream;
        emailStream << "candidate" << id << "@email.com";
        string email = emailStream.str();
        
        // Create resume with actual data
        Resume resume(id, candidateName, email, line, 2);
        extractSkills(line, nullptr, &resume);
        
        resumeList.insert(resume);
        id++;
        loaded++;
    }
    
    file.close();
}

void extractSkills(const string& text, Job* job, Resume* resume) {
    string lowerText = text;
    for (size_t i = 0; i < lowerText.length(); i++) {
        lowerText[i] = tolower(lowerText[i]);
    }
    
    for (int i = 0; i < SKILLS_COUNT; i++) {
        string lowerSkill = COMMON_SKILLS[i];
        for (size_t j = 0; j < lowerSkill.length(); j++) {
            lowerSkill[j] = tolower(lowerSkill[j]);
        }
        
        if (lowerText.find(lowerSkill) != string::npos) {
            if (job != nullptr) {
                job->addSkill(COMMON_SKILLS[i]);
            } else if (resume != nullptr) {
                resume->addSkill(COMMON_SKILLS[i]);
            }
        }
    }
}

void performMatching_LL(JobLinkedList& jobList, ResumeLinkedList& resumeList, MatchArray& matches) {
    JobNode* jobCurrent = jobList.getHead();
    
    while (jobCurrent != nullptr) {
        ResumeNode* resumeCurrent = resumeList.getHead();
        
        while (resumeCurrent != nullptr) {
            double score = MatchingEngine::calculateMatchScore(jobCurrent->data, resumeCurrent->data);
            int matchingSkills = MatchingEngine::countMatchingSkills(jobCurrent->data, resumeCurrent->data);
            
            matches.add(Match(jobCurrent->data.getId(), resumeCurrent->data.getId(), score, matchingSkills));
            
            resumeCurrent = resumeCurrent->next;
        }
        
        jobCurrent = jobCurrent->next;
    }
}

void displayTopMatches_LL(const MatchArray& matches, int top, JobLinkedList& jobList, ResumeLinkedList& resumeList) {
    if (matches.size == 0) {
        cout << "\nNo matches to display!" << endl;
        return;
    }
    
    Match* sortedMatches = new Match[matches.size];
    for (int i = 0; i < matches.size; i++) {
        sortedMatches[i] = matches.matches[i];
    }
    
    for (int i = 0; i < matches.size - 1; i++) {
        for (int j = i + 1; j < matches.size; j++) {
            if (sortedMatches[j].getScore() > sortedMatches[i].getScore()) {
                Match temp = sortedMatches[i];
                sortedMatches[i] = sortedMatches[j];
                sortedMatches[j] = temp;
            }
        }
    }
    
    int displayCount = (top < matches.size) ? top : matches.size;
    
    cout << "\nTOP " << displayCount << " MATCHES" << endl;
    cout << "===============================================" << endl;
    
    for (int i = 0; i < displayCount; i++) {
        cout << "\n[Rank #" << (i + 1) << "]" << endl;
        sortedMatches[i].display();
        
        if (i < 5) {
            Job* job = jobList.search(sortedMatches[i].getJobId());
            Resume* resume = resumeList.search(sortedMatches[i].getResumeId());
            
            if (job != nullptr && resume != nullptr) {
                MatchingEngine::displayMatchDetails(*job, *resume, sortedMatches[i].getScore());
            }
        }
    }
    
    delete[] sortedMatches;
}

void displayMenu_LL() {
    cout << "\n===============================================" << endl;
    cout << "            LINKED LIST MENU" << endl;
    cout << "===============================================" << endl;
    cout << "  1. Display All Jobs" << endl;
    cout << "  2. Display All Resumes" << endl;
    cout << "  3. Search Job by ID" << endl;
    cout << "  4. Search Resume by ID" << endl;
    cout << "  5. Sort Jobs by ID" << endl;
    cout << "  6. Sort Resumes by Experience" << endl;
    cout << "  7. Perform Complete Job Matching" << endl;
    cout << "  8. Display Top Matches" << endl;
    cout << "  9. Match Specific Job with All Resumes" << endl;
    cout << " 10. Match Specific Resume with All Jobs" << endl;
    cout << " 11. Display Performance Metrics" << endl;
    cout << "  0. Return to Main Menu" << endl;
    cout << "===============================================" << endl;
}

void displayPerformanceMetrics_LL(long long loadTime, int dataSize) {
    cout << "\nData Size: " << dataSize << " records" << endl;
    cout << "Load Time: " << loadTime << " microseconds (" 
         << fixed << setprecision(2) << (loadTime / 1000.0) << " ms)" << endl;
    
    cout << "\nCOMPLEXITY ANALYSIS:" << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "Operation          | Time        | Space" << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "Insert (End)       | O(1)        | O(1)" << endl;
    cout << "Insert (Beginning) | O(1)        | O(1)" << endl;
    cout << "Search             | O(n)        | O(1)" << endl;
    cout << "Delete             | O(n)        | O(1)" << endl;
    cout << "Sort (Bubble)      | O(n^2)      | O(1)" << endl;
    cout << "Matching           | O(n*m)      | O(1)" << endl;
    cout << "-----------------------------------------------" << endl;
    
    cout << "\nADVANTAGES OF LINKED LIST:" << endl;
    cout << "  - Dynamic size (no fixed capacity)" << endl;
    cout << "  - Efficient insertion at beginning: O(1)" << endl;
    cout << "  - Easy deletion without shifting" << endl;
    cout << "  - Memory allocated as needed" << endl;
    
    cout << "\nDISADVANTAGES OF LINKED LIST:" << endl;
    cout << "  - Slower search: O(n) linear time" << endl;
    cout << "  - Extra memory for pointers" << endl;
    cout << "  - No random access" << endl;
    cout << "  - Cache performance issues" << endl;
    cout << "===============================================\n" << endl;
}