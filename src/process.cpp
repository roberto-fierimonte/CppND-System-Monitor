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
  utilisation = LinuxParser::ProcessorUtilization(pid_);;
  ram = LinuxParser::Ram(pid_);
  uptime = LinuxParser::UpTime() - LinuxParser::UpTime(pid_);
};

int Process::Pid() { 
  return pid_; 
}

float Process::CpuUtilization() { 
  return utilisation;
}

string Process::Command() { 
  return command;
}

string Process::Ram() { 
  return ram; 
}

string Process::User() { 
  return user; 
}

long int Process::UpTime() { 
  return uptime; 
}

bool Process::operator<(Process const& a) const { 
  return (utilisation < a.utilisation); 
}