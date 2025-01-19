#pragma once
#include <Arduino.h>
/*
Class to save timers more optimal, 
Basic: saved seconds num on uint32 (4bytes)
Class: saved on struct "year:day:hour:min:sec" (3bytes)

Max timer value = 65 year - 1 sec

*/

class KTime
{
public:
    #pragma pack(push,1)
    struct data
    {
        byte year : 6;  
        uint day : 9;
        byte hour : 5; 
        byte min : 6;
        byte sec : 6;   
    } time{0, 0, 0, 0, 0};
    KTime *timer = nullptr;
    #pragma pack(pop)

    KTime();
    KTime(KTime *timer);
    void Zero();
    void Set();
    void AddTime(byte sec = 0, byte min = 0, byte hour = 0, uint day = 0, byte year=0);
    unsigned long Minutes() const;
    String ToString() const;
    bool Check() const;
};