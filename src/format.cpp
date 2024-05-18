#include <string>
#include <sstream>  // For std::stringstream
#include <iomanip>  // For std::setw and std::setfill

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int hours = seconds / 3600;
    int minutes = (seconds %3600) / 60;
    int secs = seconds % 60;

    std::stringstream ss; // Create a stringstream object
    ss << std::setw(2) << std::setfill('0') << hours << ":" // std::setw(2) sets the width of the output to 2 characters
       << std::setw(2) << std::setfill('0') << minutes << ":"
       << std::setw(2) << std::setfill('0') << secs;

    return ss.str();
}