#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "processor.h"

using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal,
      guest;
  std::ifstream stream("/proc/stat");
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest;
  }
  int Idle = std::stoi(idle) + std::stoi(iowait);
  int NonIdle = std::stoi(user) + std::stoi(nice) + std::stoi(system) +
                std::stoi(irq) + std::stoi(softirq) + std::stoi(steal);

  float Total = Idle + NonIdle;

  // differentiate: actual value minus the prev_ious one
  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;
  float CPU_Percentage = (totald - idled) / totald;

  PrevIdle = Idle;
  PrevNonIdle = NonIdle;
  PrevTotal = Total;

  return CPU_Percentage;
}
