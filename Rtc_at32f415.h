#include "RtcBase.h"
#include "at32f415_ertc.h"

class Rtc_at32f415: public RtcBase
{
    public:
        void Init()
        {
            //тут инициализация конкретно для at32f415
            crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE); /* enable the PWC clock */
            pwc_battery_powered_domain_access(TRUE); /* allow access to E_RTC */

          if ( CRM->bpdc_bit.lexten == FALSE )
          {
            /* reset E_RTC BPR domain */
            crm_battery_powered_domain_reset(TRUE);
            crm_battery_powered_domain_reset(FALSE);
            /* enable the LEXT OSC */
            crm_clock_source_enable(CRM_CLOCK_SOURCE_LEXT, TRUE);

            /* wait till LEXT is ready */
          while ( crm_flag_get(CRM_LEXT_STABLE_FLAG) == RESET ){}
            crm_ertc_clock_select(CRM_ERTC_CLOCK_LEXT)/* select the E_RTC clock source */;
            crm_ertc_clock_enable(TRUE)/* enable the E_RTC clock */;
            ertc_reset()/* reset the E_RTC registers */;
            ertc_wait_update()/* wait for E_RTC APB registers update */;
            /* configure the E_RTC divider */
            ertc_divider_set(127, 255)/* E_RTC second(1Hz) = E_RTC_clk / (div_a + 1) * (div_b + 1) */;
            ertc_hour_mode_set(ERTC_HOUR_MODE_24)/* configure the E_RTC hour mode */;
            //return 1/* ФАКТ РАЗРЯДА РЕЗЕРВНОГО ИСТОЧНИКА (БАТАРЕЙКА 3,0 В) */;
          }
        }
        void Read(Tm* tm)
        {
          ertc_time_type time_struct;
          ertc_calendar_get(&time_struct);  // Получение текущего времени

          
          tm->hour = time_struct.hour;
          tm->min = time_struct.min;
          tm->sec = time_struct.sec;
          tm->date = time_struct.day;
          tm->mon = time_struct.month;
          tm->year = time_struct.year;
        }
        void Write(Tm* time)
        {
          ertc_time_set(time->hour, time->min, time->sec, ERTC_24H);
        }
        void WriteDate(Tm* time)
        {
          ertc_date_set(time->year, time->mon, time->date, 1);
        }
        void Calibrate(int32_t value)
        {

        }
};