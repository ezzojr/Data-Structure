#include "CSVReader.hpp"
#include <fstream>
#include <cstring>

// Count lines in a file
int CSVReader::countLines(const char* filename) {
    std::ifstream file(filename);
    int count = 0;
    char ch;
    
    while (file.get(ch)) {
        if (ch == '\n') count++;
    }
    
    file.close();
    return count;
}

// Parse a CSV line into fields
void CSVReader::parseLine(const char* line, char** fields, int maxFields) {
    int fieldIndex = 0;
    int charIndex = 0;
    bool inQuotes = false;
    
    for (int i = 0; line[i] != '\0' && fieldIndex < maxFields; i++) {
        if (line[i] == '"') {
            inQuotes = !inQuotes;
        } else if (line[i] == ',' && !inQuotes) {
            fields[fieldIndex][charIndex] = '\0';
            fieldIndex++;
            charIndex = 0;
        } else {
            fields[fieldIndex][charIndex] = line[i];
            charIndex++;
        }
    }
    fields[fieldIndex][charIndex] = '\0';
}

// Trim whitespace from string
void CSVReader::trim(char* str) {
    int start = 0;
    int end = strlen(str) - 1;
    
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') start++;
    while (end >= 0 && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n' || str[end] == '\r')) end--;
    
    int j = 0;
    for (int i = start; i <= end; i++) {
        str[j++] = str[i];
    }
    str[j] = '\0';
}

// Read entire file into buffer
bool CSVReader::readFile(const char* filename, char* buffer, int maxSize) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    
    file.read(buffer, maxSize);
    buffer[file.gcount()] = '\0';
    
    file.close();
    return true;
}