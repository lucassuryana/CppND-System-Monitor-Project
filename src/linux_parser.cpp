#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  float memTotal;
  float memFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename); // /proc/meminfo
  if (filestream.is_open()) { // if the file is open
    while (std::getline(filestream, line)) { // read the file line by line
      std::istringstream linestream(line); // create a string stream from the line
      while (linestream >> key) { // linestream >> key means read the first word of the line and store it in key
        if (key == "MemTotal:") { // if the first word is MemTotal
          linestream >> memTotal; // read the next word and store it in memTotal
        } else if (key == "MemFree:") { // if the first word is MemFree
          linestream >> memFree; // read the next word and store it in memFree
          break;
        }
      }
    }
  }
  return (memTotal - memFree) / memTotal;
}

// TODO: Read and return the system uptime
long int LinuxParser::UpTime() {
  string line; // create a string to store the line
  long int uptime; // create a long int to store the uptime
  std::ifstream filestream(kProcDirectory + kUptimeFilename); // /proc/uptime
  if (filestream.is_open()) { // if the file is open
    std::getline(filestream, line); // read the first line of the file
    std::istringstream linestream(line); // create a string stream from the line
    std::string uptimestring, idletimestring; // create two strings to store the uptime and idletime
    linestream >> uptimestring >> idletimestring; // read the first and second word of the line and store them in uptime and idletime
    uptime = std::stol(uptimestring);
  }
  return uptime; // return the uptime
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line; // create a string to store the line
  string cpu; // create a string to store the cpu
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice; // create long ints to store the jiffies
  std::ifstream filestream(kProcDirectory + kStatFilename); // /proc/stat
  if (filestream.is_open()) { // if the file is open
    std::getline(filestream, line); // read the first line of the file
    std::istringstream linestream(line); // create a string stream from the line
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice; // read the first word and the jiffies and store them in the variables
  }
  long totalJiffies = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice; // calculate the total jiffies
  return totalJiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line; // create a string to store the line
  string value; // create a string to store the value
  long utime, stime, cutime, cstime; // create long ints to store the jiffies
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename); // /proc/[pid]/stat
  if (filestream.is_open()) { // if the file is open
    std::getline(filestream, line); // read the first line of the file
    std::istringstream linestream(line); // create a string stream from the line
    for (int i = 0; i < 13; i++) { // read the first 13 words of the line
      linestream >> value; // store the word in value
    }
    linestream >> utime >> stime >> cutime >> cstime; // read the next 4 words and store them in the jiffies
  }
  long totalJiffies = utime + stime + cutime + cstime; // calculate the total jiffies
  return totalJiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line; // create a string to store the line
  string cpu; // create a string to store the cpu
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice; // create long ints to store the jiffies
  std::ifstream filestream(kProcDirectory + kStatFilename); // /proc/stat
  if (filestream.is_open()) { // if the file is open
    std::getline(filestream, line); // read the first line of the file
    std::istringstream linestream(line); // create a string stream from the line
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice; // read the first word and the jiffies and store them in the variables
  }
  long activeJiffies = user + nice + system + irq + softirq + steal; // calculate the active jiffies
  return activeJiffies; // return the active jiffies
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line; // create a string to store the line
  string cpu; //  create a string to store the cpu
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice; // create long ints to store the jiffies
  std::ifstream filestream(kProcDirectory + kStatFilename); // /proc/stat
  if (filestream.is_open()) { // if the file is open
    std::getline(filestream, line); // read the first line of the file
    std::istringstream linestream(line); // create a string stream from the line
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice; // read the first word and the jiffies and store them in the variables
  }
  return idle; // return the idle jiffies
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line; // create a string to store the line
  string cpu; // create a string to store the cpu
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice; // create long ints to store the jiffies
  std::ifstream filestream(kProcDirectory + kStatFilename); // /proc/stat
  if (filestream.is_open()) { // if the file is open
    std::getline(filestream, line); // read the first line of the file
    std::istringstream linestream(line); // create a string stream from the line
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice; // read the first word and the jiffies and store them in the variables
  }
  vector<string> cpuUtilization = {to_string(user), to_string(nice), to_string(system), to_string(idle), to_string(iowait), to_string(irq), to_string(softirq), to_string(steal), to_string(guest), to_string(guest_nice)}; // store the jiffies in a vector
  return cpuUtilization; // return the vector
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line; // create a string to store the line
  string key; // create a string to store the key
  int totalProcesses = 0; // create an int to store the total processes
  std::ifstream filestream(kProcDirectory + kStatFilename); // /proc/stat
  if (filestream.is_open()) { // if the file is open
    while (std::getline(filestream, line)) { // read the file line by line
      std::istringstream linestream(line); // create a string stream from the line
      while (linestream >> key) { // read the first word of the line
        if (key == "processes") { // if the first word is processes
          linestream >> totalProcesses; // read the next word and store it in totalProcesses
          return totalProcesses; // return the totalProcesses
        }
      }
    }
  }
  return totalProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line; // create a string to store the line
  string key; // create a string to store the key
  int runningProcesses = 0; // create an int to store the running processes
  std::ifstream filestream(kProcDirectory + kStatFilename); // /proc/stat
  if (filestream.is_open()) { // if the file is open
    while (std::getline(filestream, line)) { // read the file line by line
      std::istringstream linestream(line); // create a string stream from the line
      while (linestream >> key) { // read the first word of the line
        if (key == "procs_running") { // if the first word is procs_running
          linestream >> runningProcesses; // read the next word and store it in runningProcesses
          return runningProcesses; // return the runningProcesses
        }
      }
    }
  }
  return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
