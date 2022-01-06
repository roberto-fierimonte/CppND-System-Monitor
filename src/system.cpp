#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() : cpu_(Processor()), operating_system(LinuxParser::OperatingSystem()), kernel(LinuxParser::Kernel()) {};

Processor& System::Cpu() { 
  return cpu_; 
}

vector<Process>& System::Processes() { 
    processes_.clear();
    for (int pid: LinuxParser::Pids()) {
        Process proc(pid);
        processes_.push_back(proc);
  }
  sort(processes_.begin(), processes_.end());
  reverse(processes_.begin(), processes_.end());
  return processes_;
   
}

std::string System::Kernel() { 
  return kernel; 
}

float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() { 
  return operating_system; 
}

int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() { 
  return LinuxParser::UpTime(); 
}
