#include <vector>
#include <string>

#include "processor.h"
#include "linux_parser.h"

/*
This function is derived from the calculations provided in: 
https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
*/
float Processor::Utilization() { 
  std::vector<std::string> jiffies = LinuxParser::CpuUtilization();
    
  long Idle = std::stol(jiffies[LinuxParser::kIdle_]) + stol(jiffies[LinuxParser::kIOwait_]);
  long NonIdle = std::stol(jiffies[LinuxParser::kUser_]) + stol(jiffies[LinuxParser::kNice_]);
  NonIdle += (std::stol(jiffies[LinuxParser::kSystem_]) + stol(jiffies[LinuxParser::kIRQ_]));
  NonIdle += (std::stol(jiffies[LinuxParser::kSoftIRQ_]) + stol(jiffies[LinuxParser::kSteal_]));

  // differentiate: actual value minus the previous one
  long totald = (Idle + NonIdle) - (PrevIdle + PrevNonIdle);
  long idled = Idle - PrevIdle;
  
  PrevIdle = Idle;
  PrevNonIdle = NonIdle;

  return (float) (totald - idled) / (float) totald;
}