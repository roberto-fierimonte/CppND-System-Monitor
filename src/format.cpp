#include <string>
#include <sstream>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    int secs = seconds % 60;
    int minutes = (int) seconds / 60;
    int mins = minutes % 60;
    int hrs = (int) mins / 60;

    return std::to_string(hrs) + ":" + std::to_string(mins) + ":" + std::to_string(secs);
}