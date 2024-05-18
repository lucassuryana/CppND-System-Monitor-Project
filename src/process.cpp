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

Process::Process(int pid) : pid_(pid) {} // TODO: Return this process's ID

int Process::Pid() { return pid_;} // Return the process ID

// Return this process's CPU utilization
float Process::CpuUtilization() const {
    float activeJiffies = LinuxParser::ActiveJiffies(pid_); // Get the active jiffies for the process
    float systemUptime = LinuxParser::UpTime(); // Get the system uptime which is the time since the system started
    float processUptime = LinuxParser::UpTime(pid_); // Get the process uptime which is the time since the process started with the given pid
    float seconds = systemUptime - processUptime; // Calculate the seconds, which is the difference between the system uptime and the process uptime
    float cpuUsage = activeJiffies / sysconf(_SC_CLK_TCK) / seconds; // Calculate the CPU usage by dividing the active jiffies by the number of clock ticks per second and the seconds
    return cpuUsage;  // Return the CPU usage
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); } // Return the command that generated this process

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); } // Return this process's memory utilization

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); } // Return the user (name) that generated this process

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_); // Return the age of this process (in seconds)
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return a.CpuUtilization() < CpuUtilization(); // Overload the "less than" comparison operator for Process objects
}
