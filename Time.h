#pragma once

//#include "at32f4xx.h"  

class Time {
public:
   
    Time() : hours(0), minutes(0), seconds(0), day(0), month(0), year(0) {}

    
    /*void Init() {
        
        SysTick_Config(SystemCoreClock / 1000);
    }*/

   
    void SetTime(uint8_t h, uint8_t m, uint8_t s) {
        hours = h;
        minutes = m;
        seconds = s;
    }
    
    void SetDate(uint8_t d, uint8_t mon, uint16_t y) {
      day = d;
      month = mon;
      year = y;
    }

   
    void GetTime(uint8_t &h, uint8_t &m, uint8_t &s, uint8_t &d, uint8_t &mon, uint16_t &y) const {
        h = hours;
        m = minutes;
        s = seconds;
        d = day;
        mon = month;
        y = year;
    }

    
    void Tick() {
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
            if (minutes >= 60) {
                minutes = 0;
                hours++;
                if (hours >= 24) {
                    hours = 0;
                }
            }
        }
    }

    
    void SysTickHandler() {
        static uint16_t msCounter = 0;

        
        msCounter++;
        if (msCounter >= 1000) {  
            msCounter = 0;
            Tick();  
        }
    }

private:
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t day;
    uint8_t month;
    uint16_t year;
};


Time systemTime;



