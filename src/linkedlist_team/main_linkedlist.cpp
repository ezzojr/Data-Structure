#include "JobLinkedList.hpp"
#include "ResumeLinkedList.hpp"
#include "MatchingEngine.hpp"
#include "../shared/CSVReader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

// Function prototypes
void loadJobsFromCSV(const char* filename, JobLinkedList& jobList);
void loadResumesFromCSV(const char* filename, ResumeLinkedList& resumeList);
void performMatching(JobLinkedList& jobList, ResumeLinkedList& resumeList, MatchArray& matches);
void displayTopMatches(const MatchArray& matches, int top, JobLinkedList& jobList, ResumeLinkedList& resumeList);
void displayMenu();
void displayPerformanceMetrics(long long loadTime, long long searchTime, long long sortTime, int dataSize);

int main() {
    cout << "\n╔═══════════════════════════════════════════════════════════╗" << endl;
    cout << "║       JOB MATCHING SYSTEM - LINKED LIST IMPLEMENTATION    ║" << endl;
    cout << "║              Asia Pacific University                      ║" << endl;
    cout << "╚═══════════════════════════════════════════════════════════╝\n" << endl;
    
    JobLinkedList jobList;
    ResumeLinkedList resumeList;
    MatchArray matches(1000);
    
    auto startLoad = high_resolution_clock::now();
    
    cout << "📂 Loading data from CSV files..." << endl;
    loadJobsFromCSV("data/job_description.csv", jobList);
    loadResumesFromCSV("data/resume.csv", resumeList);
    
    auto endLoad = high_resolution_clock::now();
    long long loadTime = duration_cast<microseconds>(endLoad - startLoad).count();
    
    cout << "✓ Jobs loaded: " << jobList.getSize() << endl;
    cout << "✓ Resumes loaded: " << resumeList.getSize() << endl;
    cout << "✓ Load time: " << loadTime << " microseconds\n" << endl;
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                cout << "\n📋 Displaying all jobs..." << endl;
                jobList.display();
                break;
            }
            
            case 2: {
                cout << "\n👥 Displaying all resumes..." << endl;
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
                    cout << "\n✓ Job found!" << endl;
                    job->displayDetailed();
                    cout << "Search time: " << searchTime << " microseconds" << endl;
                } else {
                    cout << "\n✗ Job not found!" << endl;
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
                    cout << "\n✓ Resume found!" << endl;
                    resume->displayDetailed();
                    cout << "Search time: " << searchTime << " microseconds" << endl;
                } else {
                    cout << "\n✗ Resume not found!" << endl;
                }
                break;
            }
            
            case 5: {
                cout << "\n📊 Sorting jobs by ID..." << endl;
                auto startSort = high_resolution_clock::now();
                jobList.sortById();
                auto endSort = high_resolution_clock::now();
                long long sortTime = duration_cast<microseconds>(endSort - startSort).count();
                
                cout << "✓ Jobs sorted successfully!" << endl;
                cout << "Sort time: " << sortTime << " microseconds" << endl;
                jobList.display();
                break;
            }
            
            case 6: {
                cout << "\n📊 Sorting resumes by experience..." << endl;
                auto startSort = high_resolution_clock::now();
                resumeList.sortByExperience();
                auto endSort = high_resolution_clock::now();
                long long sortTime = duration_cast<microseconds>(endSort - startSort).count();
                
                cout << "✓ Resumes sorted successfully!" << endl;
                cout << "Sort time: " << sortTime << " microseconds" << endl;
                resumeList.display();
                break;
            }
            
            case 7: {
                cout << "\n🎯 Performing job matching analysis..." << endl;
                matches.clear();
                
                auto startMatch = high_resolution_clock::now();
                performMatching(jobList, resumeList, matches);
                auto endMatch = high_resolution_clock::now();
                long long matchTime = duration_cast<microseconds>(endMatch - startMatch).count();
                
                cout << "✓ Matching complete!" << endl;
                cout << "Total matches found: " << matches.size << endl;
                cout << "Matching time: " << matchTime << " microseconds\n" << endl;
                break;
            }
            
            case 8: {
                if (matches.size == 0) {
                    cout << "\n⚠ No matches found! Please perform matching first (Option 7)." << endl;
                } else {
                    int top;
                    cout << "\nEnter number of top matches to display: ";
                    cin >> top;
                    displayTopMatches(matches, top, jobList, resumeList);
                }
                break;
            }
            
            case 9: {
                int jobId;
                cout << "\nEnter Job ID to match: ";
                cin >> jobId;
                
                Job* job = jobList.search(jobId);
                if (job == nullptr) {
                    cout << "✗ Job not found!" << endl;
                    break;
                }
                
                cout << "\n🎯 Matching job with all candidates..." << endl;
                job->displayDetailed();
                
                MatchArray specificMatches(resumeList.getSize());
                ResumeNode* current = resumeList.getHead();
                
                while (current != nullptr) {
                    double score = MatchingEngine::calculateMatchScore(*job, current->data);
                    int skillCount = MatchingEngine::countMatchingSkills(*job, current->data);
                    specificMatches.add(Match(job->getId(), current->data.getId(), score, skillCount));
                    current = current->next;
                }
                
                displayTopMatches(specificMatches, 10, jobList, resumeList);
                break;
            }
            
            case 10: {
                int resumeId;
                cout << "\nEnter Resume ID to match: ";
                cin >> resumeId;
                
                Resume* resume = resumeList.search(resumeId);
                if (resume == nullptr) {
                    cout << "✗ Resume not found!" << endl;
                    break;
                }
                
                cout << "\n🎯 Matching candidate with all jobs..." << endl;
                resume->displayDetailed();
                
                MatchArray specificMatches(jobList.getSize());
                JobNode* current = jobList.getHead();
                
                while (current != nullptr) {
                    double score = MatchingEngine::calculateMatchScore(current->data, *resume);
                    int skillCount = MatchingEngine::countMatchingSkills(current->data, *resume);
                    specificMatches.add(Match(current->data.getId(), resume->getId(), score, skillCount));
                    current = current->next;
                }
                
                displayTopMatches(specificMatches, 10, jobList, resumeList);
                break;
            }
            
            case 11: {
                cout << "\n📈 PERFORMANCE METRICS - LINKED LIST IMPLEMENTATION" << endl;
                cout << "═══════════════════════════════════════════════════" << endl;
                displayPerformanceMetrics(loadTime, 0, 0, jobList.getSize() + resumeList.getSize());
                break;
            }
            
            case 0: {
                cout << "\n👋 Thank you for using the Job Matching System!" << endl;
                running = false;
                break;
            }
            
            default:
                cout << "\n✗ Invalid choice! Please try again." << endl;
        }
        
        if (running) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    
    return 0;
}

void loadJobsFromCSV(const char* filename, JobLinkedList& jobList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "✗ Error: Could not open " << filename << endl;
        return;
    }
    
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string field;
        int fieldIndex = 0;
        
        int id = 0;
        string title, company, description, skillsStr;
        int experience = 0;
        
        while (getline(ss, field, ',')) {
            field.erase(0, field.find_first_not_of(" \t\r\n"));
            field.erase(field.find_last_not_of(" \t\r\n") + 1);
            
            switch (fieldIndex) {
                case 0: id = stoi(field); break;
                case 1: title = field; break;
                case 2: company = field; break;
                case 3: description = field; break;
                case 4: skillsStr = field; break;
                case 5: experience = stoi(field); break;
            }
            fieldIndex++;
        }
        
        Job job(id, title, company, description, experience);
        
        stringstream skillStream(skillsStr);
        string skill;
        while (getline(skillStream, skill, ';')) {
            skill.erase(0, skill.find_first_not_of(" \t\r\n"));
            skill.erase(skill.find_last_not_of(" \t\r\n") + 1);
            if (!skill.empty()) {
                job.addSkill(skill);
            }
        }
        
        jobList.insert(job);
    }
    
    file.close();
}

void loadResumesFromCSV(const char* filename, ResumeLinkedList& resumeList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "✗ Error: Could not open " << filename << endl;
        return;
    }
    
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string field;
        int fieldIndex = 0;
        
        int id = 0;
        string name, email, summary, skillsStr;
        int experience = 0;
        
        while (getline(ss, field, ',')) {
            field.erase(0, field.find_first_not_of(" \t\r\n"));
            field.erase(field.find_last_not_of(" \t\r\n") + 1);
            
            switch (fieldIndex) {
                case 0: id = stoi(field); break;
                case 1: name = field; break;
                case 2: email = field; break;
                case 3: summary = field; break;
                case 4: skillsStr = field; break;
                case 5: experience = stoi(field); break;
            }
            fieldIndex++;
        }
        
        Resume resume(id, name, email, summary, experience);
        
        stringstream skillStream(skillsStr);
        string skill;
        while (getline(skillStream, skill, ';')) {
            skill.erase(0, skill.find_first_not_of(" \t\r\n"));
            skill.erase(skill.find_last_not_of(" \t\r\n") + 1);
            if (!skill.empty()) {
                resume.addSkill(skill);
            }
        }
        
        resumeList.insert(resume);
    }
    
    file.close();
}

void performMatching(JobLinkedList& jobList, ResumeLinkedList& resumeList, MatchArray& matches) {
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

void displayTopMatches(const MatchArray& matches, int top, JobLinkedList& jobList, ResumeLinkedList& resumeList) {
    if (matches.size == 0) {
        cout << "\n⚠ No matches to display!" << endl;
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
    
    cout << "\n🏆 TOP " << displayCount << " MATCHES" << endl;
    cout << "═══════════════════════════════════════════════════════════" << endl;
    
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

void displayMenu() {
    cout << "\n╔═══════════════════════════════════════════════════════════╗" << endl;
    cout << "║                     MAIN MENU                             ║" << endl;
    cout << "╠═══════════════════════════════════════════════════════════╣" << endl;
    cout << "║  1. Display All Jobs                                      ║" << endl;
    cout << "║  2. Display All Resumes                                   ║" << endl;
    cout << "║  3. Search Job by ID                                      ║" << endl;
    cout << "║  4. Search Resume by ID                                   ║" << endl;
    cout << "║  5. Sort Jobs by ID                                       ║" << endl;
    cout << "║  6. Sort Resumes by Experience                            ║" << endl;
    cout << "║  7. Perform Complete Job Matching                         ║" << endl;
    cout << "║  8. Display Top Matches                                   ║" << endl;
    cout << "║  9. Match Specific Job with All Resumes                   ║" << endl;
    cout << "║ 10. Match Specific Resume with All Jobs                   ║" << endl;
    cout << "║ 11. Display Performance Metrics                           ║" << endl;
    cout << "║  0. Exit                                                  ║" << endl;
    cout << "╚═══════════════════════════════════════════════════════════╝" << endl;
}

void displayPerformanceMetrics(long long loadTime, long long searchTime, long long sortTime, int dataSize) {
    cout << "\nData Size: " << dataSize << " records" << endl;
    cout << "Load Time: " << loadTime << " microseconds (" 
         << fixed << setprecision(2) << (loadTime / 1000.0) << " ms)" << endl;
    
    cout << "\n📊 COMPLEXITY ANALYSIS:" << endl;
    cout << "─────────────────────────────────────────────────────" << endl;
    cout << "Operation          | Time Complexity | Space Complexity" << endl;
    cout << "─────────────────────────────────────────────────────" << endl;
    cout << "Insert (End)       |     O(1)        |      O(1)" << endl;
    cout << "Insert (Beginning) |     O(1)        |      O(1)" << endl;
    cout << "Search             |     O(n)        |      O(1)" << endl;
    cout << "Delete             |     O(n)        |      O(1)" << endl;
    cout << "Sort (Bubble)      |     O(n²)       |      O(1)" << endl;
    cout << "Matching           |     O(n×m)      |      O(1)" << endl;
    cout << "─────────────────────────────────────────────────────" << endl;
    
    cout << "\n💡 ADVANTAGES OF LINKED LIST:" << endl;
    cout << "   • Dynamic size (no fixed capacity)" << endl;
    cout << "   • Efficient insertion at beginning: O(1)" << endl;
    cout << "   • Easy deletion without shifting elements" << endl;
    cout << "   • Memory allocated as needed" << endl;
    
    cout << "\n⚠ DISADVANTAGES OF LINKED LIST:" << endl;
    cout << "   • Slower search: O(n) linear time" << endl;
    cout << "   • Extra memory for pointers" << endl;
    cout << "   • No random access (must traverse)" << endl;
    cout << "   • Cache performance issues" << endl;
    cout << "═══════════════════════════════════════════════════\n" << endl;
}
