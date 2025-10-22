// ArrayImpl.cpp — Array-based Job Matching (no <vector>/<list>)
// Tailored for one-column CSVs with headers: job_description, resume
// Each row is quoted text: "...."

#include "ArrayImpl.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

namespace arr {

// ============================== Limits ===============================
static const int MAX_JOBS    = 20000;
static const int MAX_RESUMES = 20000;
static const int MAX_WORDS   = 256;

// ============================== Utilities ============================
static inline char toLowerChar(char c) { return (c >= 'A' && c <= 'Z') ? char(c + 32) : c; }

static inline std::string toLower(const std::string &s) {
    std::string out; out.reserve(s.size());
    for (char c : s) out.push_back(toLowerChar(c));
    return out;
}
static inline bool isWordChar(char c) {
    return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c=='+' || c=='#';
}
static inline std::string rtrim(std::string s){
    while (!s.empty() && (s.back()=='\r' || s.back()=='\n' || s.back()==' ' || s.back()=='\t')) s.pop_back();
    return s;
}
static inline std::string ltrim(const std::string &s){
    size_t i=0; while (i<s.size() && (s[i]==' ' || s[i]=='\t')) ++i;
    return s.substr(i);
}
static inline std::string stripQuotes(std::string s){
    s = ltrim(rtrim(s));
    if (!s.empty() && s.front()=='"' && s.back()=='"' && s.size()>=2) {
        s = s.substr(1, s.size()-2);
    }
    return s;
}

// Tiny string array (no STL)
struct StringArray {
    std::string data[MAX_WORDS];
    int _size = 0;
    void clear(){ _size = 0; }
    int  size() const { return _size; }
    std::string& operator[](int i){ return data[i]; }
    const std::string& operator[](int i) const { return data[i]; }
    void push_back(const std::string &s){ if (_size < MAX_WORDS) data[_size++] = s; }
};

static inline void tokenize(const std::string &text, StringArray &out){
    out.clear();
    std::string s = toLower(text), cur;
    for (char c : s) {
        if (isWordChar(c)) cur.push_back(c);
        else if (!cur.empty()){ out.push_back(cur); cur.clear(); }
    }
    if (!cur.empty()) out.push_back(cur);
}

static inline bool contains(const StringArray &arr, const std::string &w){
    for (int i=0;i<arr.size();i++) if (arr[i]==w) return true;
    return false;
}

// ============================== Domain ===============================
struct JobA {
    int id = 0;
    std::string title;
    std::string company;
    std::string description;
    int years = 0;
    StringArray skills;
    void display() const {
        std::cout << "[" << id << "] " << title
                  << " @ " << company << "  (" << years << " yrs)\n";
    }
    void displayDetailed() const {
        display();
        std::cout << "Skills: ";
        for (int i=0;i<skills.size();++i){
            if (i) std::cout << ", ";
            std::cout << skills.data[i];
        }
        std::cout << "\nDescription: " << description << "\n";
    }
};

struct ResumeA {
    int id = 0;
    std::string name;
    std::string email;
    std::string summary;
    int years = 0;
    StringArray skills;
    void display() const {
        std::cout << "[" << id << "] " << name
                  << " <" << email << ">  (" << years << " yrs)\n";
    }
    void displayDetailed() const {
        display();
        std::cout << "Skills: ";
        for (int i=0;i<skills.size();++i){
            if (i) std::cout << ", ";
            std::cout << skills.data[i];
        }
        std::cout << "\nSummary: " << summary << "\n";
    }
};

struct BestMatch {
    int jobId = -1;
    double score = 0.0;
    int matchedSkills = 0;
};

// Expanded skills (lowercase)
static const char* COMMON_SKILLS[] = {
    "c++","python","java","javascript","sql","html","css",
    "react","node.js","mongodb","mysql","postgresql",
    "git","docker","kubernetes","aws","azure",
    "machine learning","deep learning","nlp","pandas",
    "excel","power bi","tableau","data analysis","data cleaning",
    "statistics","tensorflow","pytorch","rest api","rest apis",
    "spring","spring boot","django","flask","angular","vue.js",
    "system design","mlops","keras","computer vision",
    "stakeholder management","user stories","product roadmap"
};
static const int SKILL_COUNT = sizeof(COMMON_SKILLS)/sizeof(COMMON_SKILLS[0]);

static void extractSkills(const std::string& text, StringArray &out){
    std::string low = toLower(text);
    out.clear();
    for (int i=0;i<SKILL_COUNT;i++){
        std::string needle = COMMON_SKILLS[i];
        if (low.find(needle) != std::string::npos){
            out.push_back(needle);
        }
    }
}

// ============================== Storage ==============================
static JobA    JOBS[MAX_JOBS];
static ResumeA RESUMES[MAX_RESUMES];
static BestMatch BEST[MAX_RESUMES];
static int J = 0, R = 0;

static long long g_load_us  = 0;
static long long g_match_us = 0;

// ============================== CSV Loader ===========================
// These are tailored for your exact CSVs:
// Header is either "job_description" or "resume"; single quoted column per row.
static int readSingleColumnQuoted(const std::string &path, std::string* out, int cap){
    std::ifstream fin(path.c_str());
    if (!fin.is_open()){
        std::cerr << "Error: Could not open " << path << "\n";
        return 0;
    }
    std::string line; bool first=true;
    int n=0;
    while (std::getline(fin, line)){
        if (first) { first=false; continue; } // skip header line
        if (line.empty()) continue;
        std::string cell = stripQuotes(line);
        if (!cell.empty() && n<cap) out[n++] = cell;
    }
    return n;
}

static void loadJobs(const char* path){
    std::string descs[MAX_JOBS];
    J = readSingleColumnQuoted(path, descs, MAX_JOBS);
    int id=1;
    for (int i=0;i<J;i++){
        std::string d = descs[i];
        // Title heuristic from "X needed with experience in ..."
        std::string title = "Position";
        size_t p1 = d.find(" needed");
        size_t p2 = d.find(" required");
        if (p1!=std::string::npos && p1>0) title = d.substr(0,p1);
        else if (p2!=std::string::npos && p2>0) title = d.substr(0,p2);

        JOBS[i].id = id++;
        JOBS[i].title = title;
        JOBS[i].company = "Tech Company";
        JOBS[i].description = d;
        JOBS[i].years = 3;
        extractSkills(d, JOBS[i].skills);
    }
}

static void loadResumes(const char* path){
    std::string descs[MAX_RESUMES];
    R = readSingleColumnQuoted(path, descs, MAX_RESUMES);
    int id=101;
    for (int i=0;i<R;i++){
        std::string d = descs[i];
        RESUMES[i].id = id++;
        RESUMES[i].name = "Candidate_" + std::to_string(RESUMES[i].id);
        RESUMES[i].email = "candidate" + std::to_string(RESUMES[i].id) + "@email.com";
        RESUMES[i].summary = d;
        RESUMES[i].years = 2;
        extractSkills(d, RESUMES[i].skills);
    }
}

// ============================== Matching =============================
static int countMatchingSkills(const JobA& job, const ResumeA& res){
    int m=0;
    for (int i=0;i<res.skills.size();++i)
        if (contains(job.skills, res.skills[i])) m++;
    return m;
}

static double matchScore(const ResumeA& res, const JobA& job){
    StringArray rw, jw;
    tokenize(res.summary, rw);
    tokenize(job.description, jw);
    if (rw.size()==0 || jw.size()==0) return 0.0;
    int overlap=0;
    for (int i=0;i<rw.size();++i) if (contains(jw, rw[i])) overlap++;
    return (100.0 * overlap) / (double)rw.size();
}

static void performFullMatching(){
    auto t1 = std::chrono::high_resolution_clock::now();

    for (int ri=0; ri<R; ++ri){
        double bestS = -1.0;
        int    bestJ = -1;
        int    bestK = 0;
        for (int ji=0; ji<J; ++ji){
            double s = matchScore(RESUMES[ri], JOBS[ji]);
            if (s > bestS){
                bestS = s;
                bestJ = JOBS[ji].id;
                bestK = countMatchingSkills(JOBS[ji], RESUMES[ri]);
            }
        }
        BEST[ri].jobId = bestJ;
        BEST[ri].score = (bestS<0?0.0:bestS);
        BEST[ri].matchedSkills = bestK;
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    g_match_us = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    // Write CSV
    std::ofstream csv("matches_array.csv");
    csv << "ResumeID,BestJobID,Score,MatchedSkills\n";
    for (int ri=0; ri<R; ++ri){
        csv << RESUMES[ri].id << "," << BEST[ri].jobId << ","
            << std::fixed << std::setprecision(2) << BEST[ri].score << ","
            << BEST[ri].matchedSkills << "\n";
    }
    csv.close();
}

// ============================== Sorting/Search =======================
static void sortJobsById(){
    for (int i=0;i<J-1;i++)
        for (int k=i+1;k<J;k++)
            if (JOBS[k].id < JOBS[i].id){ JobA t = JOBS[i]; JOBS[i]=JOBS[k]; JOBS[k]=t; }
}
static void sortResumesById(){
    for (int i=0;i<R-1;i++)
        for (int k=i+1;k<R;k++)
            if (RESUMES[k].id < RESUMES[i].id){ ResumeA t = RESUMES[i]; RESUMES[i]=RESUMES[k]; RESUMES[k]=t; }
}

static JobA*    findJob(int id){    for (int i=0;i<J;i++) if (JOBS[i].id==id) return &JOBS[i]; return nullptr; }
static ResumeA* findResume(int id){ for (int i=0;i<R;i++) if (RESUMES[i].id==id) return &RESUMES[i]; return nullptr; }

// ============================== UI Helpers ===========================
static void displayAllJobs(){
    for (int i=0;i<J;i++) JOBS[i].display();
}
static void displayAllResumes(){
    for (int i=0;i<R;i++) RESUMES[i].display();
}
static void displayTopMatches(int top){
    if (R==0){ std::cout << "No matches to display.\n"; return; }
    struct Row{ int rid; int jid; double s; int k; };
    Row* rows = new Row[R];
    for (int i=0;i<R;i++){
        rows[i] = { RESUMES[i].id, BEST[i].jobId, BEST[i].score, BEST[i].matchedSkills };
    }
    for (int i=0;i<R-1;i++)
        for (int k=i+1;k<R;k++)
            if (rows[k].s > rows[i].s){ Row t=rows[i]; rows[i]=rows[k]; rows[k]=t; }

    int show = (top<R?top:R);
    std::cout << "\nTOP " << show << " MATCHES\n";
    std::cout << "-----------------------------------------------\n";
    std::cout << std::left << std::setw(12) << "Resume ID"
              << " | " << std::setw(10) << "Best Job"
              << " | " << std::setw(7)  << "Score%"
              << " | " << "Matched\n";
    std::cout << "-----------------------------------------------\n";
    for (int i=0;i<show;i++){
        std::cout << std::left << std::setw(12) << rows[i].rid
                  << " | " << std::setw(10) << rows[i].jid
                  << " | " << std::fixed << std::setprecision(2) << std::setw(7) << rows[i].s
                  << " | " << rows[i].k
                  << "\n";
    }
    std::cout << "-----------------------------------------------\n";
    delete[] rows;
}

static void menu(){
    std::cout << "\n===============================================\n";
    std::cout << "            ARRAY IMPLEMENTATION MENU\n";
    std::cout << "===============================================\n";
    std::cout << "  1. Display All Jobs\n";
    std::cout << "  2. Display All Resumes\n";
    std::cout << "  3. Search Job by ID\n";
    std::cout << "  4. Search Resume by ID\n";
    std::cout << "  5. Sort Jobs by ID\n";
    std::cout << "  6. Sort Resumes by ID\n";
    std::cout << "  7. Perform Complete Job Matching\n";
    std::cout << "  8. Display Top Matches\n";
    std::cout << "  9. Display Performance Metrics\n";
    std::cout << "  0. Return to Main Menu\n";
    std::cout << "===============================================\n";
}

static void showPerf(){
    std::cout << "\nPerformance Summary (Array)\n";
    std::cout << "-----------------------------------------------\n";
    std::cout << "Records: Jobs=" << J << ", Resumes=" << R << "\n";
    std::cout << "CSV Loading Time : " << g_load_us  << " µs\n";
    std::cout << "Matching Time    : " << g_match_us << " µs\n";
    std::cout << "-----------------------------------------------\n";
}

} // namespace arr

// ============================== Public Entry ==========================
ArrayPerf runArrayImplementation(const char* jobsCsvPath, const char* resumesCsvPath){
    using namespace std;
    using namespace std::chrono;
    using namespace arr;

    cout << "=====================================\n";
    cout << "   ARRAY-BASED JOB MATCHING SYSTEM   \n";
    cout << "=====================================\n\n";

    auto t0 = high_resolution_clock::now();
    loadJobs(jobsCsvPath);
    loadResumes(resumesCsvPath);
    auto t1 = high_resolution_clock::now();

    g_load_us = duration_cast<microseconds>(t1 - t0).count();

    cout << "Total Jobs Loaded    : " << J << "\n";
    cout << "Total Resumes Loaded : " << R << "\n";

    int choice; bool running=true;
    while (running){
        menu();
        cout << "Enter your choice: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        switch (choice){
            case 1: cout << "\nDisplaying all jobs...\n"; displayAllJobs(); break;
            case 2: cout << "\nDisplaying all resumes...\n"; displayAllResumes(); break;
            case 3: {
                int id; cout << "Enter Job ID: "; cin >> id;
                JobA* j = findJob(id);
                if (j){ cout << "\nJob found:\n"; j->displayDetailed(); }
                else   cout << "Job not found.\n";
                break;
            }
            case 4: {
                int id; cout << "Enter Resume ID: "; cin >> id;
                ResumeA* r = findResume(id);
                if (r){ cout << "\nResume found:\n"; r->displayDetailed(); }
                else   cout << "Resume not found.\n";
                break;
            }
            case 5: cout << "Sorting jobs by ID...\n"; sortJobsById(); cout << "Jobs sorted.\n"; break;
            case 6: cout << "Sorting resumes by ID...\n"; sortResumesById(); cout << "Resumes sorted.\n"; break;
            case 7: {
    std::cout << "Performing job matching analysis...\n";
    performFullMatching();  // Run the algorithm

    std::cout << "Matching complete!\n";

    // Display table header
    std::cout << "\n--------------------------------------------\n";
    std::cout << std::left << std::setw(10) << "Resume ID"
              << " | " << std::setw(10) << "Best Job ID"
              << " | " << std::setw(7)  << "Score%"
              << " | " << "Matched Skills\n";
    std::cout << "--------------------------------------------\n";

    // Show top results neatly
    for (int ri = 0; ri < R; ++ri) {
        std::cout << std::left << std::setw(10) << RESUMES[ri].id
                  << " | " << std::setw(10) << BEST[ri].jobId
                  << " | " << std::fixed << std::setprecision(2) << std::setw(7) << BEST[ri].score
                  << " | " << BEST[ri].matchedSkills
                  << "\n";
    }

    std::cout << "--------------------------------------------\n";
    std::cout << "Total matches found: " << R << "\n";
    std::cout << "Matching time: " << g_match_us << " microseconds\n";
    std::cout << "Results saved to: matches_array.csv\n";
    break;
}

            case 8: {
                int top; cout << "Show top how many matches? "; cin >> top;
                displayTopMatches(top);
                break;
            }
            case 9: showPerf(); break;
            case 0:  running = false; break;
            default: cout << "Invalid choice.\n";
        }
        if (running){
            cout << "\nPress Enter to continue...";
            cin.ignore(); cin.get();
        }
    }

    ArrayPerf out;
    out.load_us  = g_load_us;
    out.match_us = g_match_us;
    out.jobs     = J;
    out.resumes  = R;
    return out;
}