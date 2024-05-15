#include "processor.h"
#include "linux_parser.h"
#include <string>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    // Get the CPU utilization values from LinuxParser
    std::vector<std::string> cpuUtilizationstring = LinuxParser::CpuUtilization(); // [user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice]
    // Convert the vector of strings to a vector of longs
    std::vector<long> cpuUtilization;
    for (const std::string& value : cpuUtilizationstring) {
        cpuUtilization.push_back(std::stol(value));
    }
    // Calculate the total CPU utilization
    long idle = cpuUtilization[LinuxParser::kIdle_] + cpuUtilization[LinuxParser::kIOwait_]; // idle + iowait
    long nonIdle = cpuUtilization[LinuxParser::kUser_] + cpuUtilization[LinuxParser::kNice_] +
                                 cpuUtilization[LinuxParser::kSystem_] + cpuUtilization[LinuxParser::kIRQ_] +
                                 cpuUtilization[LinuxParser::kSoftIRQ_] + cpuUtilization[LinuxParser::kSteal_]; // user + nice + system + irq + softirq + steal
    long total = idle + nonIdle; // idle + nonIdle

    // Calculate the CPU utilization percentage
    float cpuUtilizationPercentage = (total - idle) / static_cast<float>(total); // (total - idle) / total

    return cpuUtilizationPercentage; // Return the CPU utilization percentage
}

