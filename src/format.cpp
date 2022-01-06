#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
  int secs = seconds % 60;
  int minutes = (int) seconds / 60;
  int mins = minutes % 60;
  int hrs = (int) mins / 60;
  string hrs_s = (hrs < 10) ? "0" + std::to_string(hrs) : std::to_string(hrs);
  string mins_s = (mins < 10) ? "0" + std::to_string(mins) : std::to_string(mins);
  string secs_s = (secs < 10) ? "0" + std::to_string(secs) : std::to_string(secs);

  return hrs_s + ":" + mins_s + ":" + secs_s;
}