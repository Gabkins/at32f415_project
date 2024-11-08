#pragma once
#include "KeysBase.h"
#include "hardware.h"

extern uint8_t buttons;

class Keys_M128i1x: public KeysBase
{
    public:
      Keys_M128i1x() : KeysBase((uint8_t const[]){KeysBase::LONG_OPTION, KeysBase::LONG_OPTION, KeysBase::REPEAT_OPTION, KeysBase::REPEAT_OPTION}, 4){}
    void Init()
    {

    }

    uint8_t Read()
    {
        return buttons;
    }
};


