#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   // Method to return the system's CPU
  std::vector<Process>& Processes();  // Method to return a container composed of the system's processes
  float MemoryUtilization();          // Method to return the system's memory utilization
  long UpTime();                      // Method to return the number of seconds since the system started running
  int TotalProcesses();               // Method to return the total number of processes on the system
  int RunningProcesses();             // Method to return the number of processes actively running on the system
  std::string Kernel();               // Method to return the system's kernel identifier (string)
  std::string OperatingSystem();      // Method to return the operating system name

  //Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif