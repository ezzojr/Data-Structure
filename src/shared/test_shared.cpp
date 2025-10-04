#include "Job.hpp"
#include "Resume.hpp"
#include "Match.hpp"
#include <iostream>

int main() {
    std::cout << "Testing Shared Classes..." << std::endl << std::endl;
    
    // Test Job
    Job job1(1, "Software Engineer", "Tech Corp", 
             "Develop C++ applications", 3);
    job1.addSkill("C++");
    job1.addSkill("Python");
    job1.addSkill("Git");
    
    std::cout << "JOB TEST:" << std::endl;
    job1.displayDetailed();
    std::cout << std::endl;
    
    // Test Resume
    Resume resume1(101, "John Doe", "john@email.com",
                   "Experienced developer", 5);
    resume1.addSkill("C++");
    resume1.addSkill("Java");
    resume1.addSkill("Git");
    
    std::cout << "RESUME TEST:" << std::endl;
    resume1.displayDetailed();
    std::cout << std::endl;
    
    // Test Match
    Match match1(1, 101, 75.5, 2);
    std::cout << "MATCH TEST:" << std::endl;
    match1.display();
    
    return 0;
}