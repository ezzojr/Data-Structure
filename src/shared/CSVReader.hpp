#ifndef CSVREADER_HPP
#define CSVREADER_HPP

#include <string>

class CSVReader {
public:
    // Helper functions to parse CSV
    static int countLines(const char* filename);
    static void parseLine(const char* line, char** fields, int maxFields);
    static void trim(char* str);
    static bool readFile(const char* filename, char* buffer, int maxSize);
};

#endif