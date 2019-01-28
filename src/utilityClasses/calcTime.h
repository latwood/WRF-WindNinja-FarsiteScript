#ifndef CALCTIME_H
#define CALCTIME_H

#include <iostream>
#include <vector>

#include <chrono>

class calcTime
{
public:

    // constructor
    calcTime();

    // reconstructor like functions
    void reset();

    // set value functions
    void startNewTime(std::string newTimeName);

    // get value functions
    void getAndPrint_time(std::string storedTimeName);

private:

    // utility functions
    bool doesTimeExist(std::string newTImeName);
    size_t findTimeIdx(std::string storedTimeName);
    void calcTimeDistribution(double totalSeconds, int &milliseconds, int &seconds, int &minutes, int &hours, int &days);
    void printTimes(int milliseconds, int seconds, int minutes, int hours, int days, std::string timerName);

    // data members
    std::vector<std::string> storedTimeNames;
    std::vector<std::chrono::time_point<std::chrono::_V2::system_clock,std::chrono::duration<long,std::ratio<1,1000000000> > > > storedStartTimes;

};

#endif // CALCTIME_H
