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

//Constructor for the Process class
Process::Process(int pid) : pid_(pid) {};


// DONE: Return this process's ID
int Process::Pid() {
    return pid_;
}

void Process::CpuUsage(){
    cpu_utilization = Process::CpuUtilization();
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization(){
   long curr_proc_jiff = LinuxParser::ActiveJiffies(pid_);
   long curr_sys_jiff = LinuxParser::Jiffies();
   long recently_active_ProcessJiffies = curr_proc_jiff - previously_active_jiffies_;
   long recently_active_SystemJiffies = curr_sys_jiff - previous_system_jiffies_;
   float cpu_utilization = static_cast<float>(recently_active_ProcessJiffies) / recently_active_SystemJiffies;
   previously_active_jiffies_ = curr_proc_jiff;
   previous_system_jiffies_ = curr_sys_jiff;
   return cpu_utilization;
}

// DONE: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(pid_);    
}

// DONE: Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(pid_);
}

// DONE: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(pid_);
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid_);
}

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {    
    return cpu_utilization < a.cpu_utilization;       
}

bool Process::operator>(Process const& a) const {    
    return cpu_utilization > a.cpu_utilization;       
}