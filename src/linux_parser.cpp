#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  string mem_total{};
  string mem_free{};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        if (key == "MemTotal:") {
          mem_total = value;
        } else if (key == "MemFree:") {
          mem_free = value;
          break;
        }
      }
    }
  }
  float total_mem = std::stof(mem_total);
  float total_free = std::stof(mem_free);
  return ((total_mem - total_free) / total_mem);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string up_time, idle_time, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time >> idle_time;
  }
  float result = std::stof(up_time);
  return static_cast<long int>(result);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, temp, utime, stime, cutime, cstime, start_time;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) +
                       kStatFilename);
  float cpu_usage{0};

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    for (int i = 0; i < 13; i++) {
      linestream >> temp;
    }
    linestream >> utime >> stime >> cutime >> cstime >> temp >> temp >> temp >>
        temp >> start_time >> temp;

    float total_time = stof(utime) + stof(stime) + stof(cutime) + stof(cstime);
    float seconds = UpTime() - (stof(start_time) / sysconf(_SC_CLK_TCK));
    cpu_usage = 100 * (total_time / sysconf(_SC_CLK_TCK)) / seconds;
  }
  return cpu_usage;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string processes, count, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> processes) {
        if (processes == "processes") {
          linestream >> count;
          return std::stoi(count);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string processes, count, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> processes) {
        if (processes == "procs_running") {
          linestream >> count;
          return std::stoi(count);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) {
  string line, command;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) +
                       kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, temp, ram;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) +
                       kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      if (line.find("VmSize:") != std::string::npos) {
        linestream >> temp >> ram;
        return std::to_string(std::stoi(ram) / 1000);
      }
    }
  }
  return "Unknown";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, temp, user;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) +
                       kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      if (line.find("Uid:") != std::string::npos) {
        linestream >> temp >> user;
        return user;
      }
    }
  }
  return "Unknown";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, temp, user;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      if (line.find(":" + Uid(pid) + ":") != std::string::npos) {
        return line.substr(0, line.find(":"));
      }
    }
  }
  return "Unknown";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string value, line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) +
                       kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int counter = 0; counter < 22; counter++) {
      linestream >> value;
    }
  }
  return UpTime() - (std::stol(value) / sysconf(_SC_CLK_TCK));
}
