#include "at32f415_conf.h"
#include "at32f415_int.h"
#include "SystemInit.h"
#include "Indication_M128i1x.h"
#include "Time.h"
#include "Keys_M128i1x.h"
#include "Rtc_at32f415.h"

Indication_M128i1x indicator;
Keys_M128i1x keys;
Rtc_at32f415 time;
Tm currentTime;
Tm setTime;

uint8_t hours, minutes, seconds, day, month;
uint8_t showDate = 0;
uint8_t settingMode = 0;
uint16_t year = 2024;
/*
hours = 12;
minutes, seconds, day, month = 0;
year = 2024;*/

void SystickHandler()
{
  systemTime.SysTickHandler();
  indicator.Handler();
  keys.Handler();
  time.Init();

  
}

KeysBase::Event ev;
void KeysLoop()
{
  ev = keys.GetEvent();

  if (ev.type == KeysBase::PRESSED)
  {
    switch (ev.value)
    {
      case 0x01:    //change showing date
        if (!settingMode)
        {
          showDate = (showDate == 2) ? 0 : showDate + 1;
        }
        break;

      case 0x02:  // 2 button, 
        if (settingMode)
        {
          switch (showDate)
          {
            case 0:  // time
              currentTime.hour = (currentTime.hour + 1) % 24;
              time.Write(&currentTime);
              //systemTime.SetTime(hours, minutes, seconds);
              break;

            case 1:  // date
              currentTime.date++;
              if (currentTime.date > 31)
              {
                currentTime.date = 1;
                
              }
              //systemTime.SetDate(day, month, year);
              time.WriteDate(&currentTime);
              break;

            case 2:  // year
              currentTime.year++;
              time.WriteDate(&currentTime);
              //systemTime.SetDate(day, month, year);
              break;
          }
        }
        break;

      case 0x04:  // 3 button, 
        if (settingMode)
        {
          switch (showDate)
          {
            case 0:  // time
              currentTime.min++;
              if (currentTime.min >= 60)
              {
                currentTime.min = 0;
              }
              time.Write(&currentTime);
              //systemTime.SetTime(hours, minutes, seconds);
              break;

            case 1:  // date 
              currentTime.mon = (currentTime.mon % 12) + 1;
              time.WriteDate(&currentTime);
              //systemTime.SetDate(day, month, year);
              break;

            case 2:  // year
              currentTime.year--;
              time.WriteDate(&currentTime);
              //systemTime.SetDate(day, month, year);
              break;
          }
        }
        break;

      case 0x08:  // 4 button, turn on/off setting mode
        settingMode = !settingMode;  // 
        indicator.SetLed(0, 1);
        indicator.SetLed(1, 0);
        indicator.SetLed(2, 1);
        break;

      default:
        break;
    }
  }
}



void main()
{
  systemInit();
  indicator.Init();
  keys.Init();
  
  time.Read(&currentTime);
  
  //systemTime.SetTime(12, 0, 0);
  //systemTime.SetDate(01, 1, 2024);
  
  while(true)
  {
    time.Read(&currentTime);
    systemTime.GetTime(hours, minutes, seconds, day, month, year);
    if (showDate == 0)
    {
      //indicator.Print(0, "%02d%02d", hours, minutes);
      indicator.Print(0, "%02d%02d", currentTime.hour, currentTime.min);
    } else if (showDate == 1)
      {
        indicator.Print(0, "%02d%02d", currentTime.date, currentTime.mon);
      } else 
        {
          indicator.Print(0, "%d", currentTime.year);
        }
    

    KeysLoop();
    //indicator.Print(0, "%02d%02d", day, month);

 
  }
}
