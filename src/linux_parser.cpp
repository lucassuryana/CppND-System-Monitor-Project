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

// Read data from the filesystem
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

// Read data from the filesystem
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

// Method to return pids of all the processes
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

//method to return the memory utilization
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

// Method to return the uptime of the system
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

// Method to return the number of jiffies
// Jiffies are the number of ticks that have occurred since the system started
// Tick is a basic unit of time in computing
// We use jiffies to measure the time the CPU has spent on different tasks
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

// Read and return the number of active jiffies for a PID
// Each PID is associated with a process
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

// Read and return the number of active jiffies for the system
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

// Read and return the number of idle jiffies for the system
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

// Read and return CPU utilization
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

// Read and return the total number of processes
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

// Read and return the number of running processes
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

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line; // create a string to store the line
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename); // /proc/[pid]/cmdline
  if (filestream.is_open()) { // if the file is open
    std::getline(filestream, line); // read the first line of the file
    return line; // return the command
  }
  return string(); // return an empty string if the file cannot be opened
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line; // create a string to store the line
  string key; // create a string to store the key
  string value; // create a string to store the value
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename); // /proc/[pid]/status
  if (filestream.is_open()) { // if the file is open
    while (std::getline(filestream, line)) { // read the file line by line
      std::istringstream linestream(line); // create a string stream from the line
      linestream >> key >> value; // read the first and second word of the line and store them in key and value
      if (key == "VmSize:") { // if the first word is VmSize
        long ramValue = std::stol(value); // convert value to long
        ramValue /= 1000; // divide by 1000
        return std::to_string(ramValue); // convert back to string and return
        return value; // return the value
      }
    }
  }
  return string(); // return an empty string if the file cannot be opened or the key is not found
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line; // create a string to store the line
  string key; // create a string to store the key
  string uid; // create a string to store the user ID
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename); // /proc/[pid]/status
  if (filestream.is_open()) { // if the file is open
    while (std::getline(filestream, line)) { // read the file line by line
      std::istringstream linestream(line); // create a string stream from the line
      linestream >> key >> uid; // read the first and second word of the line and store them in key and uid
      if (key == "Uid:") { // if the first word is Uid
        return uid; // return the user ID
      }
    }
  }
  return string(); // return an empty string if the file cannot be opened or the key is not found
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line; // create a string to store the line
  string key; // create a string to store the key
  string uid = Uid(pid); // get the user ID associated with the process
  std::ifstream filestream(kPasswordPath); // /etc/passwd
  if (filestream.is_open()) { // if the file is open
    while (std::getline(filestream, line)) { // read the file line by line
      std::replace(line.begin(), line.end(), ':', ' '); // replace ':' with ' ' to split the line into tokens
      std::istringstream linestream(line); // create a string stream from the line
      string username, x, userUid; // create strings to store the username and user ID
      linestream >> username >> x >> userUid; // read the first, second, and third token of the line and store them in username, x, and userUid
      if (userUid == uid) { // if the user ID matches the process's user ID
        return username; // return the username
      }
    }
  }
  return string(); // return an empty string if the file cannot be opened or the user is not found
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line; // create a string to store the line
  string value; // create a string to store the value
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename); // /proc/[pid]/stat
  if (filestream.is_open()) { // if the file is open
    std::getline(filestream, line); // read the first line of the file
    std::istringstream linestream(line); // create a string stream from the line
    for (int i = 0; i < 22; ++i) { // iterate through the first 22 words
      linestream >> value; // read the next word and store it in value
    }
    long startTime = std::stol(value); // convert value to long
    long processUptime = (startTime / sysconf(_SC_CLK_TCK)); // calculate the process uptime
    return processUptime; // return the process uptime
  }
  return 0; // return 0 if the file cannot be opened
}
