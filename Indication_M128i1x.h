#pragma once
#include "IndicationBase.h"
#include "hardware.h"


extern const unsigned char symbols[124];
extern uint8_t buttons;

class Indication_M128i1x : public IndicationBase
{
private:
    uint8_t _digits[MaxDigits()];
    uint8_t GetCode(const char c)
    {
        unsigned char i = 0;
        while (i < 124)
        {
            if (symbols[i] == c)
                return symbols[i + 1];
            i += 2;
        }
        return 0;
    }

    void digs_off()
    {
        uint8_t i;
        io_clr(STR);
        io_clr(CLK);
        io_set(DATA);
        for (i = 0; i < 8; i++)
        {
            io_set(CLK);
            io_clr(CLK);
        }
        io_set(STR);
    }

    void dig_on(uint8_t dig)
    {
        io_clr(STR);
        io_clr(CLK);
        for (int8_t i = 8; i >= 0; i--)
        {
            if (i == dig)
                io_clr(DATA);
            else
                io_set(DATA);
            io_set(CLK);
            io_clr(CLK);
        }
        io_set(STR);
    }

    void checkButton(uint8_t num)
    {
        if (!io_check(KN))
            buttons |= (1 << num);
        else
            buttons &= ~(1 << num);
    }

    void fixPointPosition()
    {
        if (digit[1] & 0x80)
        {
            _digits[1] &= ~(0x80);
            _digits[0] |= 0x80;
        }
        else
            _digits[0] &= ~(0x80);
    }

public:
    void Init()
    {
        crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
        gpio_pin_remap_config(SWJTAG_GMUX_010, TRUE);

        crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
        crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
        crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
        crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
        crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);

        gpio_init_type GPIO_InitStructure;
        GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
        GPIO_InitStructure.gpio_mode = GPIO_MODE_OUTPUT;
        GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
        GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;

        GPIO_InitStructure.gpio_pins = PIN(LDD);
        gpio_init(PORT(LDD), &GPIO_InitStructure);

        GPIO_InitStructure.gpio_pins = PIN(LDC);
        gpio_init(PORT(LDC), &GPIO_InitStructure);

        GPIO_InitStructure.gpio_pins = PIN(STR);
        gpio_init(PORT(STR), &GPIO_InitStructure);

        GPIO_InitStructure.gpio_pins = PIN(DATA);
        gpio_init(PORT(DATA), &GPIO_InitStructure);

        GPIO_InitStructure.gpio_pins = PIN(CLK);
        gpio_init(PORT(CLK), &GPIO_InitStructure);

        GPIO_InitStructure.gpio_pins = PIN(KN);
        GPIO_InitStructure.gpio_mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
        gpio_init(PORT(KN), &GPIO_InitStructure);
    }
    void Show()
    {
        static uint8_t digitNum;
        digs_off();
        for (uint8_t i = 0; i < MaxDigits(); i++)
            _digits[i] = digit[i];
        fixPointPosition();
        for (uint8_t i = 0; i < 8; i++)
        {
            if (_digits[digitNum] & (0x80 >> i))
                io_clr(LDD); // LDD=0;
            else
                io_set(LDD); // LDD=1;
            io_set(LDC);     // LDC=1;
            io_clr(LDC);     // LDC=0;
        }

        dig_on(digitNum);
        checkButton(digitNum);

        if (digitNum == (MaxDigits() - 1))
            digitNum = 0;
        else
            digitNum++;
        // if (brightnessToDelay(bright_current))
        //    startTimer(brightnessToDelay(bright_current));
    }
};
