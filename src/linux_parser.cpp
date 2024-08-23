#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector> 
#include <algorithm>

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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  std::string line;
  int line_num = 0;
  std::string token; 
  std::vector<std::string> mem;
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      while (linestream >> token){
        mem.push_back(token);
      }
      line_num++;
      if (line_num == 2){
        break;
      }
    }
  }
  float mem_total = std::stof(mem[1]);
  float mem_free = std::stof(mem[4]);
  float mem_used = (mem_total - mem_free)/1000; // used memory in MB
  return mem_used*100/mem_total;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
  std::string line;
  std::string token;
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> token;
  }
  long total_uptime = std::stol(token);
  return total_uptime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long total_jiffies = ActiveJiffies() + IdleJiffies();
  return total_jiffies;
}

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::string line;
  std::ifstream filestream(LinuxParser::kProcDirectory + "/" + std::to_string(pid) + LinuxParser::kStatFilename);
  std::getline(filestream, line);
  std::string token;
  std::vector<std::string> token_vector;
  std::istringstream linestream(line);
  while (linestream >> token){
    token_vector.push_back(token);
  }
  long active_jiffies = std::stol(token_vector[13]) + std::stol(token_vector[14]) + std::stol(token_vector[15]) + std::stol(token_vector[16]);
  active_jiffies = active_jiffies ;/// sysconf(_SC_CLK_TCK);
  return active_jiffies;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  // we could have done this without using the enums, but since they;re defined already we use them
  std::vector<std::string> cpu_states = CpuUtilization();
  long active_jiffies = std::stol(cpu_states[CPUStates::kUser_]) + 
                        std::stol(cpu_states[CPUStates::kNice_]) + 
                        std::stol(cpu_states[CPUStates::kSystem_]) + 
                        std::stol(cpu_states[CPUStates::kIRQ_]) + 
                        std::stol(cpu_states[CPUStates::kSoftIRQ_]) + 
                        std::stol(cpu_states[CPUStates::kSteal_]);
  return active_jiffies;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::vector<std::string> cpu_states = CpuUtilization();
  long idle_jiffies = std::stol(cpu_states[CPUStates::kIdle_]) + std::stol(cpu_states[CPUStates::kIOwait_]);
  return idle_jiffies;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  std::ifstream file_stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string line; // variable to store line contents
  std::string key_req, key;
  std::vector<std::string> cpu_states;
  if (file_stream.is_open()){
      while (std::getline(file_stream, line)){
          std::istringstream linestream(line);
          while (linestream >> key){
              if (key == "cpu"){
                  key_req = key;
                while (linestream >> key){
                  cpu_states.push_back(key);
                } 
              }
          }
      }
  }
  return cpu_states;  
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream file_stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string line; // variable to store line contents
  std::string key, processes, num_processes;  
  if (file_stream.is_open()){
    while (std::getline(file_stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes"){
        linestream >> num_processes;
      } 
    }
  }
  return std::stoi(num_processes); 
} 

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream file_stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string line; // variable to store line contents
  std::string key, processes, num_processes;  
  if (file_stream.is_open()){
    while (std::getline(file_stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running"){
        linestream >> num_processes;
      } 
    }
  }
  return std::stoi(num_processes);
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string line; 
  std::ifstream filestream(LinuxParser::kProcDirectory + "/" + std::to_string(pid) + LinuxParser::kCmdlineFilename);
  std::getline(filestream, line);
  return line;
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::string proc_status = LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename;
  std::ifstream filestream(proc_status);
  std::string line;
  std::string key, ram;
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:"){
        linestream >> ram;
      }
    }
  }
  return std::to_string(std::stol(ram)/1000);
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::string proc_status = LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename;
  std::ifstream filestream(proc_status);
  std::string line;
  std::string key, uid;
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:"){
        linestream >> uid;
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string username_, x_, uid_;
  std::string username;
  std::string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(LinuxParser::kPasswordPath);
  std::string line; 
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), ',', ' ');
      std::istringstream linestream(line);
      linestream >> username_ >> x_ >> uid_;
      if (uid_ == uid) {
        username = username_;
      }
    }
  }
  return username;
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::string line;
  std::ifstream filestream(LinuxParser::kProcDirectory + "/" + std::to_string(pid) + LinuxParser::kStatFilename);
  std::getline(filestream, line);
  std::string token;
  std::vector<std::string> token_vector;
  std::istringstream linestream(line);
  while (linestream >> token){
    token_vector.push_back(token);
  }
  long uptime = LinuxParser::UpTime() - std::stol(token_vector[21])/sysconf(_SC_CLK_TCK);
  return uptime;
}
