
#include "calcTime.h"

/***** public functions *****/

/*** constructor functions ***/
calcTime::calcTime()
{
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void calcTime::reset()
{
    while(!storedTimeNames.empty())
    {
        storedTimeNames.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
void calcTime::startNewTime(std::string newTimeName)
{
    if(doesTimeExist(newTimeName) == true)
    {
        printf("Invalid newTimeName %s! newTimeName already exists so can't start a new time with the same name! Exiting Program!\n",newTimeName.c_str());
        exit(1);
    }
    storedTimeNames.push_back(newTimeName);
    storedStartTimes.push_back(std::chrono::high_resolution_clock::now()); // start recording execution time
}
/*** end set value functions ***/

/*** get value functions ***/
void calcTime::getAndPrint_time(std::string storedTimeName)
{
    // first set time this function is called to get a difference between the desired start time and the current time
    auto nowTime = std::chrono::high_resolution_clock::now();  // finish recording execution time

    // now find if time exists and which index
    size_t foundTimeIdx = findTimeIdx(storedTimeName);
    auto foundStartTime = storedStartTimes[foundTimeIdx];

    // now get the total number of seconds passed
    std::chrono::duration<double> elapsed = nowTime - foundStartTime;

    // now figure out how to convert total seconds into different time period amounts
    int passedMilliseconds;
    int passedSeconds = 0;
    int passedMinutes = 0;
    int passedHours = 0;
    int passedDays = 0;
    calcTimeDistribution(elapsed.count(), passedMilliseconds, passedSeconds, passedMinutes, passedHours, passedDays);

    // now print off whatever times are needed, depending on what is zero or not
    printTimes(passedMilliseconds, passedSeconds, passedMinutes, passedHours, passedDays,storedTimeName);

}
/*** end get value functions ***/



/***** private functions *****/



/*** utility functions ***/
bool calcTime::doesTimeExist(std::string newTImeName)
{
    bool foundTimeName = false;
    for(size_t timeIdx = 0; timeIdx < storedTimeNames.size(); timeIdx++)
    {
        if(storedTimeNames[timeIdx] == newTImeName)
        {
            foundTimeName = true;
            break;
        }
    }

    return foundTimeName;
}

size_t calcTime::findTimeIdx(std::string storedTimeName)
{
    bool foundTimeName = false;
    size_t foundTimeIdx = 0;
    for(size_t timeIdx = 0; timeIdx < storedTimeNames.size(); timeIdx++)
    {
        if(storedTimeNames[timeIdx] == storedTimeName)
        {
            foundTimeName = true;
            foundTimeIdx = timeIdx;
            break;
        }
    }

    if(doesTimeExist(storedTimeName) == false)
    {
        printf("Invalid storedTimeName %s! storedTimeName does not exist in calcTime storage! Exiting Program!\n",storedTimeName.c_str());
        exit(1);
    }

    return foundTimeIdx;
}

void calcTime::calcTimeDistribution(double totalSeconds, int &milliseconds, int &seconds, int &minutes, int &hours, int &days)
{
    days = totalSeconds/(24*3600);
    totalSeconds = totalSeconds - (24*3600*days);
    hours = totalSeconds/(3600);
    totalSeconds = totalSeconds - (3600*hours);
    minutes = totalSeconds/60;
    totalSeconds = totalSeconds - (60*minutes);
    seconds  = totalSeconds;
    milliseconds = (totalSeconds - (seconds))*1000;
}

void calcTime::printTimes(int milliseconds, int seconds, int minutes, int hours, int days, std::string timerName)
{
    printf("\nElapsed time for \"%s\" timer: ",timerName.c_str());
    if(days != 0)
    {
        printf("%d days, ", days);
    }
    if(hours != 0)
    {
        printf("%d hours, ", hours);
    }
    if(minutes != 0)
    {
        printf("%d minutes, ", minutes);
    }
    printf("%d seconds, %d milliseconds\n\n", seconds, milliseconds);

}
/*** end utility functions ***/
