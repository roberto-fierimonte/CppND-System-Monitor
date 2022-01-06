#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <regex>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
	string line;
	string key;
	string value;
	std::ifstream filestream(kOSPath);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ' ', '_');
			std::replace(line.begin(), line.end(), '=', ' ');
			std::replace(line.begin(), line.end(), '"', ' ');
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "PRETTY_NAME") {
					std::replace(value.begin(), value.end(), '_', ' ');
					return value;
				}
			}
		}
	}
	return value;
}

string LinuxParser::Kernel() {
	string os, kernel, version;
	string line;
	std::ifstream stream(kProcDirectory + kVersionFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> os >> version >> kernel;
	}
	return kernel;
}

/* Keeping this version here for reference */
// vector<int> LinuxParser::Pids() {
//   vector<int> pids;
//   DIR* directory = opendir(kProcDirectory.c_str());
//   struct dirent* file;
//   while ((file = readdir(directory)) != nullptr) {
//     // Is this a directory?
//     if (file->d_type == DT_DIR) {
//       // Is every character of the name a digit?
//       string filename(file->d_name);
//       if (std::all_of(filename.begin(), filename.end(), isdigit)) {
//         int pid = stoi(filename);
//         pids.push_back(pid);
//       }
//     }
//   }
//   closedir(directory);
//   return pids;
// }

/* 
Installed GCC / G++ v9.4 in order to make this work. Guide:
https://askubuntu.com/questions/466651/how-do-i-use-the-latest-gcc-on-ubuntu
*/
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  string dir_name;
  for (auto const& dir_entry : std::filesystem::directory_iterator(kProcDirectory)) {
    dir_name = dir_entry.path().filename();
    if ((dir_entry.is_directory()) && (std::all_of(dir_name.begin(), dir_name.end(), isdigit))) {
      int pid = stoi(dir_name);
      pids.push_back(pid);
    }
  }
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string key, kb;
  string line;
  unsigned long mem_total, mem_free, mem_available, buffers;
  string prefix1 = "MemTotal:";
  string prefix2 = "MemFree:";
  string prefix3 = "MemAvailable:";
  string prefix4 = "Buffers:";
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      if (!line.compare(0, prefix1.size(), prefix1)) {
        linestream >> key >> mem_total >> kb;
      } 
      else if (!line.compare(0, prefix2.size(), prefix2)) {
        linestream >> key >> mem_free >> kb;
      }
      else if (!line.compare(0, prefix3.size(), prefix3)) {
        linestream >> key >> mem_available >> kb;
      }
      else if (!line.compare(0, prefix4.size(), prefix4)) {
        linestream >> key >> buffers >> kb;
      }
    }
  }
  return (float) (mem_total - mem_free - buffers) / (float) mem_total;
}

long LinuxParser::UpTime() { 
  long uptime1, uptime2;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime1 >> uptime2;
  }
  return uptime1 + uptime2; 
}

long LinuxParser::Jiffies() { 
  // unused
  return 0; 
}

long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
  // unused
  return 0; 
}

long LinuxParser::ActiveJiffies() { 
  // unused
  return 0;
}

long LinuxParser::IdleJiffies() { 
  // unused
  return 0;
}

vector<string> LinuxParser::CpuUtilization() {
  /*
  This helper function is inspired by:
  https://stackoverflow.com/questions/9139300/stringstream-to-vectorint
  */
  auto is_number = [](const string& s) -> bool {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) {
      ++it;
    }
   	return !s.empty() && it == s.end();
    };
  
  vector<string> res;
  string line;
  
//   string line, cpu;
//   long usertime, nicetime, systemtime, idletime;
//   long ioWait, irq, softIrq, steal, guest, guestnice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string tmpbuffer;
    while (linestream >> tmpbuffer) {
      if (is_number(tmpbuffer)) {
        res.push_back(tmpbuffer);
      }
    }
//     linestream >> cpu >> usertime >> nicetime >> systemtime >> idletime >> ioWait >> irq >> softIrq >> steal >> guest >> guestnice;
    
//     long Idle = idletime + ioWait;
//     long NonIdle = usertime + nicetime + systemtime + irq + softIrq + steal;
//     long Total = Idle + NonIdle;

//     return {std::to_string((float) (Total - Idle) / (float) Total)};
  }
//   return {string()};
  return res;
}

int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  string prefix = "processes";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (!line.compare(0, prefix.size(), prefix)) {
        std::istringstream linestream(line);
        linestream >> key >> value;
        return value;
      }
    }
  }
  return value;
}

int LinuxParser::RunningProcesses() { 
  string line;
  string key; 
  int value;
  string prefix = "procs_running";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (!line.compare(0, prefix.size(), prefix)) {
        std::istringstream linestream(line);
        linestream >> key >> value;
        return value;
      }
    }
  }
  return value;
}

string LinuxParser::Command(int pid) { 
  string command;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command;
}

string LinuxParser::Ram(int pid) { 
  string line;
  string key, kb; 
  int value;
  string prefix = "VmSize:";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (!line.compare(0, prefix.size(), prefix)) {
        std::istringstream linestream(line);
        linestream >> key >> value;
        return to_string(value / 1024);
      }
    }
  }
  return string();
}

string LinuxParser::Uid(int pid) { 
  string line;
  string key, value1, value2, value3, value4;
  string prefix = "Uid:";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (!line.compare(0, prefix.size(), prefix)) {
        std::istringstream linestream(line);
        linestream >> key >> value1 >> value2 >> value3 >> value4;
        return value1;
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line;
  std::regex e ("^.*(?=:x:" + uid + ":)");
  
  std::ifstream stream(kPasswordPath);
  while (std::getline(stream, line)) {
    std::smatch sm;
    std::regex_search(line, sm, e);
    if (sm.size() == 1) {
      return sm[0];
    }
  }
  return string(); 
}

long LinuxParser::UpTime(int pid) { 
  string line;
  std::regex e ("(?:[^\\s]*\\s){21}([0-9]*)");
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::smatch sm;
    if (std::regex_search(line, sm, e)) {
      return std::stol(sm[1]) / sysconf(_SC_CLK_TCK);
    }
  }
  return 0; 
}

/* This is a new helper function that I declared also in the header file */
float LinuxParser::ProcessorUtilization(int pid) {
  string line, line2, uptime1, uptime2;
  int uptime, utime, stime, cutime, cstime, starttime;
  std::regex e1 ("(?:[^\\s]*\\s){13}([0-9]*)");
  std::regex e2 ("(?:[^\\s]*\\s){14}([0-9]*)");
  std::regex e3 ("(?:[^\\s]*\\s){15}([0-9]*)");
  std::regex e4 ("(?:[^\\s]*\\s){16}([0-9]*)");
  std::regex e5 ("(?:[^\\s]*\\s){21}([0-9]*)");
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::smatch sm;
      if (std::regex_search(line, sm, e1)) {
        utime = std::stoi(sm[1]);
      }
      if (std::regex_search(line, sm, e2)) {
        stime = std::stoi(sm[1]);
      }
      if (std::regex_search(line, sm, e3)) {
        cutime = std::stoi(sm[1]);
      }
      if (std::regex_search(line, sm, e4)) {
        cstime = std::stoi(sm[1]);
      }
      if (std::regex_search(line, sm, e5)) {
        starttime = std::stoi(sm[1]);
      }
    }
  }
  std::ifstream stream2(kProcDirectory + kUptimeFilename);
  if (stream2.is_open()) {
    std::getline(stream2, line2);
    std::istringstream linestream2(line2);
    linestream2 >> uptime1 >> uptime2;
    uptime = stoi(uptime1);
  }
  
  int total_time = utime + stime + cutime + cstime;
  float seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
  return ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
}