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

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  std::vector<int> pids = LinuxParser::Pids();
  if (!pids.empty()) {
    for (unsigned int i = 0; i < pids.size(); i++) {
      Process process(pids[i]);
      processes_.emplace_back(process);
    }
    std::sort(processes_.begin(), processes_.end());
  }
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
// check if this has to be read from the meminfo page
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
//@todo somhow this is not printed in monitor but value is correct
long int System::UpTime() { return LinuxParser::UpTime(); }
