#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hours = seconds / 3600;
    long mins = (seconds / 60) - (hours*60);
    long secs = seconds - (mins*60) - (hours*3600);
    std::string hms = std::to_string(hours) + ":" + std::to_string(mins) + ":" + std::to_string(secs);
    return hms;
}