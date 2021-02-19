#include "Time.h"

// Callum Mackenzie

Time* Time::time;

Time* Time::getTime()
{
    if (time == nullptr) {
        time = new Time();
    }
    return time;
}
