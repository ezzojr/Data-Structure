#ifndef ARRAYIMPL_HPP
#define ARRAYIMPL_HPP

// Performance tracking structure for array implementation
struct ArrayPerf {
    long long load_us;    // CSV loading time in microseconds
    long long match_us;   // Matching algorithm time in microseconds
    int jobs;             // Number of jobs loaded
    int resumes;          // Number of resumes loaded
    
    // Constructor to initialize values
    ArrayPerf() : load_us(0), match_us(0), jobs(0), resumes(0) {}
};

// Main entry point for array-based job matching system
// Parameters:
//   jobsCsvPath    - Path to jobs CSV file (format: id,title,company,description,skills,experience)
//   resumesCsvPath - Path to resumes CSV file (format: id,name,email,summary,skills,experience)
// Returns:
//   ArrayPerf structure containing performance metrics and data counts
// Features:
//   - Loads CSV data into array-based structures
//   - Interactive menu for searching, sorting, and matching
//   - Generates matches_array.csv output file
//   - Tracks performance metrics for comparison with linked list implementation
ArrayPerf runArrayImplementation(const char* jobsCsvPath, const char* resumesCsvPath);

#endif // ARRAYIMPL_HPP