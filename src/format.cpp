#include <iomanip>
#include <sstream>
#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) {
  std::stringstream result;
  long hours = seconds / 3600;
  long remaining_seconds = seconds % 3600;
  long minutes = remaining_seconds / 60;
  long last_seconds = remaining_seconds % 60;
  result << std::setw(2) << std::setfill('0') << hours << ":" << minutes << ":"
         << last_seconds;
  return result.str();
}
