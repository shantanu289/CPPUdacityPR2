#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

void System::PopulateProcesses(){
    std::vector<int> active_pids = LinuxParser::Pids();
    processes_.clear();
    int len = active_pids.size();
    for (int i = 0; i < len; i++){
        Process proc(active_pids[i]);
        processes_.push_back(proc); 
    }
}

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    System::PopulateProcesses();
    std::sort(processes_.begin(), processes_.end(), std::greater<>());
    return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
    return LinuxParser::Kernel();
}

// DONE: Return the system's memory utilization
float System::MemoryUtilization() {
    return LinuxParser::MemoryUtilization();
}

// DONE: Return the operating system name
std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}

// DONE: Return the total number of processes on the system
int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();
}

// DONE: Return the number of seconds since the system started running
long int System::UpTime() {
    return LinuxParser::UpTime();
}