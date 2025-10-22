#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <cctype>

// Include both implementations
#include "array_team/ArrayImpl.hpp"
#include "linkedlist_team/JobLinkedList.hpp"
#include "linkedlist_team/ResumeLinkedList.hpp"
#include "linkedlist_team/MatchingEngine.hpp"

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
void displayMainMenu();
void comparePerformance(ArrayPerf arrayPerf, long long llLoadTime, long long llMatchTime, int llJobs, int llResumes);

// Common tech skills to look for
const string COMMON_SKILLS[] = {
    "C++", "Python", "Java", "JavaScript", "SQL", "HTML", "CSS",
    "React", "Node.js", "MongoDB", "MySQL", "PostgreSQL",
    "Git", "Docker", "Kubernetes", "AWS", "Azure", "GCP",
    "Machine Learning", "Deep Learning", "NLP", "Pandas", "NumPy",
    "Excel", "Power BI", "Tableau", "Data Analysis", "Data Cleaning",
    "Statistics", "TensorFlow", "PyTorch", "REST API",
    "Spring", "Django", "Flask", "Angular", "Vue.js",
    "System Design", "MLOps", "Keras", "Computer Vision",
    "Stakeholder Management", "User Stories", "Product Roadmap",
    "Agile", "Scrum", "Reporting", "BI", "ETL", "Data Warehouse",
    "Leadership", "Communication", "Problem Solving", "Teamwork"
};
const int SKILLS_COUNT = 52;

// Global variables for performance tracking
long long g_llLoadTime = 0;
long long g_llMatchTime = 0;
int g_llJobs = 0;
int g_llResumes = 0;

int main() {
    cout << "\n===============================================" << endl;
    cout << "    JOB MATCHING SYSTEM - DSTR PROJECT" << endl;
    cout << "         Asia Pacific University" << endl;
    cout << "===============================================\n" << endl;
    
    ArrayPerf arrayPerf;
    bool arrayRan = false;
    bool linkedListRan = false;
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMainMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                cout << "\n>>> Running Array Implementation...\n" << endl;
                arrayPerf = runArrayImplementation("data/job_description.csv", 
                                                  "data/resume.csv");
                arrayRan = true;
                break;
            }
            
            case 2: {
                cout << "\n>>> Running Linked List Implementation...\n" << endl;
                runLinkedListImplementation();
                linkedListRan = true;
                break;
            }
            
            case 3: {
                if (!arrayRan && !linkedListRan) {
                    cout << "\nPlease run at least one implementation first!" << endl;
                } else {
                    cout << "\n>>> Performance Comparison\n" << endl;
                    comparePerformance(arrayPerf, g_llLoadTime, g_llMatchTime, g_llJobs, g_llResumes);
                }
                break;
            }
            
            case 0: {
                cout << "\nThank you for using the Job Matching System!" << endl;
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
    
    return 0;
}

void displayMainMenu() {
    cout << "\n===============================================" << endl;
    cout << "     SELECT DATA STRUCTURE IMPLEMENTATION" << endl;
    cout << "===============================================" << endl;
    cout << "  1. Array Implementation" << endl;
    cout << "  2. Linked List Implementation" << endl;
    cout << "  3. Compare Both Implementations" << endl;
    cout << "  0. Exit" << endl;
    cout << "===============================================" << endl;
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
    loadJobsFromCSV_LL("data/job_description.csv", jobList);
    loadResumesFromCSV_LL("data/resume.csv", resumeList);
    
    auto endLoad = high_resolution_clock::now();
    g_llLoadTime = duration_cast<microseconds>(endLoad - startLoad).count();
    
    g_llJobs = jobList.getSize();
    g_llResumes = resumeList.getSize();
    
    cout << "\nJobs loaded: " << g_llJobs << endl;
    cout << "Resumes loaded: " << g_llResumes << endl;
    cout << "Load time: " << fixed << setprecision(2) << (g_llLoadTime/1000.0) << " ms\n" << endl;
    
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
                cout << "This will compare " << g_llResumes << " resumes with " << g_llJobs << " jobs...\n" << endl;
                matches.clear();
                
                auto startMatch = high_resolution_clock::now();
                performMatching_LL(jobList, resumeList, matches);
                auto endMatch = high_resolution_clock::now();
                g_llMatchTime = duration_cast<microseconds>(endMatch - startMatch).count();
                
                cout << "\nMatching complete!" << endl;
                cout << "Total matches found: " << matches.size << endl;
                cout << "Matching time: " << fixed << setprecision(2) << (g_llMatchTime/1000.0) << " ms" << endl;
                
                // Save to CSV
                ofstream csv("matches_linkedlist.csv");
                csv << "JobID,ResumeID,Score,MatchedSkills\n";
                for (int i = 0; i < matches.size; i++) {
                    csv << matches.matches[i].getJobId() << ","
                        << matches.matches[i].getResumeId() << ","
                        << fixed << setprecision(2) << matches.matches[i].getScore() << ","
                        << matches.matches[i].getMatchingSkills() << "\n";
                }
                csv.close();
                cout << "Results saved to: matches_linkedlist.csv\n" << endl;
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
                displayPerformanceMetrics_LL(g_llLoadTime, g_llJobs + g_llResumes);
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
    getline(file, line); // Skip header
    
    int id = 1;
    int count = 0;
    
    cout << "Loading jobs: ";
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        // Clean the line
        while (!line.empty() && (line.front() == '"' || line.front() == ' ')) {
            line = line.substr(1);
        }
        while (!line.empty() && (line.back() == '"' || line.back() == '\r' || 
                                 line.back() == '\n' || line.back() == ' ')) {
            line.pop_back();
        }
        
        if (line.length() < 20) continue;
        
        // Progress indicator
        if (count % 50 == 0) {
            cout << ".";
            cout.flush();
        }
        
        // Extract title
        string title = "Position";
        size_t neededPos = line.find(" needed");
        size_t requiredPos = line.find(" required");
        
        if (neededPos != string::npos && neededPos > 0 && neededPos < 100) {
            title = line.substr(0, neededPos);
        } else if (requiredPos != string::npos && requiredPos > 0 && requiredPos < 100) {
            title = line.substr(0, requiredPos);
        }
        
        Job job(id, title, "Tech Company", line, 3);
        extractSkills(line, &job, nullptr);
        
        jobList.insert(job);
        id++;
        count++;
    }
    
    cout << " Done!" << endl;
    file.close();
}

void loadResumesFromCSV_LL(const char* filename, ResumeLinkedList& resumeList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        return;
    }
    
    string line;
    getline(file, line); // Skip header
    
    int id = 101;
    int count = 0;
    
    cout << "Loading resumes: ";
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        // Clean the line
        while (!line.empty() && (line.front() == '"' || line.front() == ' ')) {
            line = line.substr(1);
        }
        while (!line.empty() && (line.back() == '"' || line.back() == '\r' || 
                                 line.back() == '\n' || line.back() == ' ')) {
            line.pop_back();
        }
        
        if (line.length() < 20) continue;
        
        // Progress indicator
        if (count % 50 == 0) {
            cout << ".";
            cout.flush();
        }
        
        string candidateName = "Candidate_" + to_string(id);
        string email = "candidate" + to_string(id) + "@email.com";
        
        Resume resume(id, candidateName, email, line, 2);
        extractSkills(line, nullptr, &resume);
        
        resumeList.insert(resume);
        id++;
        count++;
    }
    
    cout << " Done!" << endl;
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

// OPTIMIZED: Added progress indicator
void performMatching_LL(JobLinkedList& jobList, ResumeLinkedList& resumeList, MatchArray& matches) {
    int totalJobs = jobList.getSize();
    int totalResumes = resumeList.getSize();
    int processedJobs = 0;
    int progressStep = totalJobs / 10;
    if (progressStep == 0) progressStep = 1;
    
    cout << "Matching progress: ";
    
    JobNode* jobCurrent = jobList.getHead();
    
    while (jobCurrent != nullptr) {
        // Show progress
        if (processedJobs % progressStep == 0) {
            cout << (processedJobs * 100 / totalJobs) << "% ";
            cout.flush();
        }
        
        ResumeNode* resumeCurrent = resumeList.getHead();
        
        while (resumeCurrent != nullptr) {
            double score = MatchingEngine::calculateMatchScore(jobCurrent->data, resumeCurrent->data);
            int matchingSkills = MatchingEngine::countMatchingSkills(jobCurrent->data, resumeCurrent->data);
            
            matches.add(Match(jobCurrent->data.getId(), resumeCurrent->data.getId(), score, matchingSkills));
            
            resumeCurrent = resumeCurrent->next;
        }
        
        jobCurrent = jobCurrent->next;
        processedJobs++;
    }
    
    cout << "100% Done!" << endl;
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
    
    // Bubble sort by score
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
    cout << "Matching (Optimized) | O(n*m*k)  | O(1)" << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "where k = average skills per entity (~10-15)" << endl;
    
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

void comparePerformance(ArrayPerf arrayPerf, long long llLoadTime, long long llMatchTime, int llJobs, int llResumes) {
    cout << "\n===============================================" << endl;
    cout << "         PERFORMANCE COMPARISON" << endl;
    cout << "===============================================\n" << endl;
    
    // Check what data is available
    bool hasArrayData = (arrayPerf.jobs > 0);
    bool hasLLData = (llJobs > 0);
    bool hasArrayMatching = (arrayPerf.match_us > 0);
    bool hasLLMatching = (llMatchTime > 0);
    
    if (!hasArrayData && !hasLLData) {
        cout << "No performance data available!" << endl;
        cout << "Please run both implementations first:\n" << endl;
        cout << "1. Select option 1 (Array Implementation)" << endl;
        cout << "2. Run option 7 to perform matching" << endl;
        cout << "3. Return to main menu (option 0)" << endl;
        cout << "4. Select option 2 (Linked List Implementation)" << endl;
        cout << "5. Run option 7 to perform matching" << endl;
        cout << "6. Return to main menu (option 0)" << endl;
        cout << "7. Select option 3 to compare" << endl;
        return;
    }
    
    // Display side-by-side comparison table
    cout << "================================================================" << endl;
    cout << left << setw(30) << "METRIC" 
         << setw(20) << "ARRAY" 
         << setw(20) << "LINKED LIST" << endl;
    cout << "================================================================" << endl;
    
    // Data Size
    cout << left << setw(30) << "Jobs Loaded";
    if (hasArrayData) {
        cout << setw(20) << arrayPerf.jobs;
    } else {
        cout << setw(20) << "Not Run";
    }
    if (hasLLData) {
        cout << setw(20) << llJobs;
    } else {
        cout << setw(20) << "Not Run";
    }
    cout << endl;
    
    cout << left << setw(30) << "Resumes Loaded";
    if (hasArrayData) {
        cout << setw(20) << arrayPerf.resumes;
    } else {
        cout << setw(20) << "Not Run";
    }
    if (hasLLData) {
        cout << setw(20) << llResumes;
    } else {
        cout << setw(20) << "Not Run";
    }
    cout << endl;
    
    cout << "----------------------------------------------------------------" << endl;
    
    // Loading Time
    cout << left << setw(30) << "Loading Time (ms)";
    if (hasArrayData) {
        cout << setw(20) << fixed << setprecision(2) << (arrayPerf.load_us / 1000.0);
    } else {
        cout << setw(20) << "Not Run";
    }
    if (hasLLData) {
        cout << setw(20) << fixed << setprecision(2) << (llLoadTime / 1000.0);
    } else {
        cout << setw(20) << "Not Run";
    }
    cout << endl;
    
    // Matching Time
    cout << left << setw(30) << "Matching Time (ms)";
    if (hasArrayMatching) {
        cout << setw(20) << fixed << setprecision(2) << (arrayPerf.match_us / 1000.0);
    } else if (hasArrayData) {
        cout << setw(20) << "Run Option 7";
    } else {
        cout << setw(20) << "Not Run";
    }
    if (hasLLMatching) {
        cout << setw(20) << fixed << setprecision(2) << (llMatchTime / 1000.0);
    } else if (hasLLData) {
        cout << setw(20) << "Run Option 7";
    } else {
        cout << setw(20) << "Not Run";
    }
    cout << endl;
    
    // Total Time
    cout << left << setw(30) << "Total Time (ms)";
    if (hasArrayMatching) {
        double totalArray = (arrayPerf.load_us + arrayPerf.match_us) / 1000.0;
        cout << setw(20) << fixed << setprecision(2) << totalArray;
    } else if (hasArrayData) {
        cout << setw(20) << "Incomplete";
    } else {
        cout << setw(20) << "Not Run";
    }
    if (hasLLMatching) {
        double totalLL = (llLoadTime + llMatchTime) / 1000.0;
        cout << setw(20) << fixed << setprecision(2) << totalLL;
    } else if (hasLLData) {
        cout << setw(20) << "Incomplete";
    } else {
        cout << setw(20) << "Not Run";
    }
    cout << endl;
    
    cout << "================================================================\n" << endl;
    
    // Performance Analysis
    if (hasArrayData && hasLLData) {
        cout << "SPEED COMPARISON:" << endl;
        cout << "----------------------------------------------------------------" << endl;
        
        // Loading comparison
        if (arrayPerf.load_us > 0 && llLoadTime > 0) {
            double loadRatio = (double)arrayPerf.load_us / llLoadTime;
            cout << "Loading: ";
            if (loadRatio > 1.05) {
                cout << "Linked List is " << fixed << setprecision(1) 
                     << ((loadRatio - 1) * 100) << "% faster";
            } else if (loadRatio < 0.95) {
                cout << "Array is " << fixed << setprecision(1) 
                     << ((1.0/loadRatio - 1) * 100) << "% faster";
            } else {
                cout << "Both are approximately equal";
            }
            cout << endl;
        }
        
        // Matching comparison
        if (hasArrayMatching && hasLLMatching) {
            double matchRatio = (double)arrayPerf.match_us / llMatchTime;
            cout << "Matching: ";
            if (matchRatio > 1.05) {
                cout << "Linked List is " << fixed << setprecision(1) 
                     << ((matchRatio - 1) * 100) << "% faster";
            } else if (matchRatio < 0.95) {
                cout << "Array is " << fixed << setprecision(1) 
                     << ((1.0/matchRatio - 1) * 100) << "% faster";
            } else {
                cout << "Both are approximately equal";
            }
            cout << endl;
        } else {
            cout << "Matching: Run option 7 in both implementations to compare" << endl;
        }
        
        // Overall comparison
        if (hasArrayMatching && hasLLMatching) {
            double totalArray = arrayPerf.load_us + arrayPerf.match_us;
            double totalLL = llLoadTime + llMatchTime;
            double totalRatio = totalArray / totalLL;
            
            cout << "Overall: ";
            if (totalRatio > 1.05) {
                cout << "Linked List is " << fixed << setprecision(1) 
                     << ((totalRatio - 1) * 100) << "% faster overall";
            } else if (totalRatio < 0.95) {
                cout << "Array is " << fixed << setprecision(1) 
                     << ((1.0/totalRatio - 1) * 100) << "% faster overall";
            } else {
                cout << "Both implementations have similar performance";
            }
            cout << endl;
        }
        cout << "----------------------------------------------------------------\n" << endl;
    }
    
    // Algorithm Complexity Table
    cout << "ALGORITHM COMPLEXITY COMPARISON:" << endl;
    cout << "================================================================" << endl;
    cout << left << setw(30) << "Operation" 
         << setw(20) << "Array" 
         << setw(20) << "Linked List" << endl;
    cout << "================================================================" << endl;
    cout << left << setw(30) << "Insert at End"
         << setw(20) << "O(1)*" 
         << setw(20) << "O(1)" << endl;
    cout << left << setw(30) << "Insert at Beginning"
         << setw(20) << "O(n)" 
         << setw(20) << "O(1)" << endl;
    cout << left << setw(30) << "Search by ID"
         << setw(20) << "O(n)" 
         << setw(20) << "O(n)" << endl;
    cout << left << setw(30) << "Delete"
         << setw(20) << "O(n)" 
         << setw(20) << "O(n)" << endl;
    cout << left << setw(30) << "Sort (Bubble)"
         << setw(20) << "O(n²)" 
         << setw(20) << "O(n²)" << endl;
    cout << left << setw(30) << "Access by Index"
         << setw(20) << "O(1)" 
         << setw(20) << "O(n)" << endl;
    cout << left << setw(30) << "Matching (Optimized)"
         << setw(20) << "O(n*m*k)" 
         << setw(20) << "O(n*m*k)" << endl;
    cout << left << setw(30) << "Memory Overhead"
         << setw(20) << "Lower" 
         << setw(20) << "Higher" << endl;
    cout << "================================================================" << endl;
    cout << "*O(1) amortized; O(n) when resizing needed" << endl;
    cout << "k = average number of skills per entity\n" << endl;
    
    // Memory Analysis
    cout << "MEMORY COMPARISON:" << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << "Array Implementation:" << endl;
    cout << "  - Contiguous memory allocation" << endl;
    cout << "  - Better cache locality" << endl;
    cout << "  - Fixed overhead per element" << endl;
    cout << "  - May waste space if not full" << endl;
    cout << "\nLinked List Implementation:" << endl;
    cout << "  - Scattered memory allocation" << endl;
    cout << "  - Pointer overhead (8 bytes per node)" << endl;
    cout << "  - No wasted space for unused capacity" << endl;
    cout << "  - Dynamic memory allocation" << endl;
    cout << "================================================================\n" << endl;
    
    // Recommendations
    cout << "RECOMMENDATIONS:" << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << "Use Array when:" << endl;
    cout << "  - Need fast random access by index" << endl;
    cout << "  - Data size is known or predictable" << endl;
    cout << "  - Memory locality is important" << endl;
    cout << "  - Frequent lookups/reads" << endl;
    cout << "\nUse Linked List when:" << endl;
    cout << "  - Frequent insertions at beginning" << endl;
    cout << "  - Unknown or highly variable data size" << endl;
    cout << "  - Need true dynamic sizing" << endl;
    cout << "  - Memory fragmentation is acceptable" << endl;
    cout << "================================================================\n" << endl;
    
    // Show what's missing
    if (!hasArrayData || !hasLLData || !hasArrayMatching || !hasLLMatching) {
        cout << "NOTE: For complete comparison, please:" << endl;
        if (!hasArrayData) {
            cout << "  - Run Array Implementation (Option 1)" << endl;
        } else if (!hasArrayMatching) {
            cout << "  - Run matching in Array Implementation (Option 1 > 7)" << endl;
        }
        if (!hasLLData) {
            cout << "  - Run Linked List Implementation (Option 2)" << endl;
        } else if (!hasLLMatching) {
            cout << "  - Run matching in Linked List Implementation (Option 2 > 7)" << endl;
        }
        cout << endl;
    }
}