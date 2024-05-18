#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);                        // Constructor for Process class
  int Pid();                               // Method to return the process ID
  std::string User();                      // Method to return the user name
  std::string Command();                   // Method to return the command name
  float CpuUtilization() const;            // Method to return the CPU utilization
  std::string Ram();                       // Method to return the RAM usage
  long int UpTime();                       // Method to return the uptime
  bool operator<(Process const& a) const;  // Method to overload the "less than" comparison operator for Process objects, ampersand is used to pass the object by reference. A const keyword is used to make sure that the object is not modified.

// Declare any necessary private members
// Private members are needed to store the process ID, user name, command name, CPU utilization, RAM usage, and uptime.
 private:
    int pid_;              // Process ID
    std::string user_;     // User name
    std::string command_;  // Command name
    float cpu_utilization_;  // CPU utilization
    std::string ram_;        // RAM usage
    long int uptime_;        // Uptime
};

#endif