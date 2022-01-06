#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
   public:
      Process(int pid);
      int Pid();
      std::string User();
      std::string Command();
      float CpuUtilization();
      std::string Ram();
      long int UpTime();
      bool operator<(Process const& a) const;

   private:
      int pid_{0};
      std::string user;
      std::string command;
      float utilisation;
      long uptime;
      std::string ram;
};

#endif