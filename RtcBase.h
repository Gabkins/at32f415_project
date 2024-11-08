#pragma once
#include "stdint.h"

class Tm
{
    public:
    uint8_t sec, min, hour, date, mon, year;

    //Тут будут разные методы для работы с датой и временем. Например преобразование в количеству секунд от 1 января 2000 года,
    //преобразование времени в строку в разных форматах, прибавление вычитание времени и т.д.

};

class RtcBase
{
public:
    //Это виртуальные методы, которые необходимо реализовать в классах наследниках
    virtual void Init() = 0;
    virtual void Read(Tm* tm) = 0;
    virtual void Write(Tm* tm) = 0;
    virtual void WriteDate(Tm* tm) = 0;
    virtual void Calibrate(int32_t value) = 0;
};




