#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
    user = LinuxParser::User(pid);
    command = LinuxParser::Command(pid);
    path = LinuxParser::kProcDirectory + to_string(pid);
}

int Process::Pid() { 
    return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    return 0; 
}

string Process::Command() { 
    return command; 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return string(); 
}

string Process::User() { 
    return user; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return 0; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }