#include "KTime.h"
#include <Arduino.h>

/// <summary>
/// Timer initialization 
/// </summary>
/// <param name="timer"> link to control-timer</param>
KTime::KTime(KTime *timer)
{
    this->timer = timer;
}

KTime::KTime()
{}

//Set timer to zero posion
void KTime::Zero()
{
    time.year = 0;
    time.day = 0;
    time.hour = 0;
    time.min = 0;
    time.sec = 0;
}

//Сheck if the timer is not zero
boolean KTime::Check() const
{
    if (time.sec != 0) return true;
    if (time.min != 0) return true;
    if (time.hour != 0) return true;
    if (time.day != 0) return true;
    if (time.year != 0) return true;
    return false;
}

//Set equal value as on control-timer
void KTime::Set()
{
    if (timer)
    {
        time.year = timer->time.year;
        time.day = timer->time.day;
        time.hour = timer->time.hour;
        time.min = timer->time.min;
        time.sec = timer->time.sec;
    }
}

//Increase timer on delta
void KTime::AddTime(byte sec, byte min, byte hour, uint day, byte year)
{
    if (timer) return;
    if (sec + time.sec >= 60)
    {
        time.sec = (sec + time.sec) % 60;
        min++;
    }
    else time.sec += sec;

    if (min + time.min >= 60)
    {
        time.min = (min + time.min) % 60;
        hour++;
    }
    else time.min += min;

    if (hour + time.hour >= 24)
    {
        time.hour = (hour + time.hour) % 24;
        day++;
    }
    else time.hour += hour;

    if (day + time.day >= 365)
    {
        time.day = (day + time.day) % 365;
        year++;
    }
    else time.day += day;

    time.year += year;
    
}

//Timer value on minutes (uint32)
unsigned long KTime::Minutes() const
{
    if (!Check()) return 0;
    if (timer)
    {
        return ((uint32_t)timer->time.year - time.year) * 365 * 24 * 60
                + ((uint32_t) timer->time.day - time.day) * 24 * 60
                + ((uint32_t) timer->time.hour - time.hour) * 60
                + ((uint32_t) timer->time.min - time.min);
    }
    return  (uint32_t)time.year * 365 * 24 * 60 + time.day * 24 * 60 + time.hour * 60 + time.min;
}

//Get timer on string-format "year:day:hour:min:sec"
String KTime::ToString() const
{
    String out="";
    if (!Check()) return "0:0:0:0";
    if (timer)
    {
        byte sec = 0, min = 0, hour = 0, year = 0;
        int day = 0;
        if (timer->time.sec < time.sec)
        {
            sec = (60 - time.sec + timer->time.sec);
            min++;
        }
        else sec = timer->time.sec - time.sec;

        if (timer->time.min < time.min + min)
        {
            min = (60 - time.min - min + timer->time.min);
            hour++;
        }
        else min = timer->time.min - time.min - min;

        if (timer->time.hour < time.hour + hour)
        {
            hour = (24 - time.hour - hour + timer->time.hour);
            day++;
        }
        else hour = timer->time.hour - time.hour - hour;

        if (timer->time.day < time.day + day)
        {
            day = (24 - time.day - day + timer->time.day);
            year++;
        }
        else day = timer->time.day - time.day - day;
        year += time.year;

        if(year!=0) out += String(year) + ":";
        out += String(day);
        out += ":" + String(hour);
        out += ":" + String(min);
        out += ":" + String(sec);
    }
    else
    {
        if (time.year != 0) out +=  String(time.year) + ":";
        out += String(time.day);
        out += ":" + String(time.hour);
        out += ":" + String(time.min);
        out += ":" + String(time.sec);
    }
    return out;
}

