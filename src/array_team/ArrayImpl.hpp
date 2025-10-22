#pragma once
#include <cstdint>

// Simple perf summary so main menu can show/compare timings
struct ArrayPerf {
    long long load_us  = 0;   // CSV loading time (µs)
    long long match_us = 0;   // Matching time (µs) after you run "Perform matching"
    int jobs    = 0;
    int resumes = 0;
};

// Runs the array-based implementation with its own submenu (like Linked List).
// - Loads CSVs
// - Presents the same menu: display/search/sort/match/show-top/perf
// - Writes "matches_array.csv" when you perform matching
// Returns timings so you can compare with the LL part later if you want.
ArrayPerf runArrayImplementation(const char* jobsCsvPath, const char* resumesCsvPath);