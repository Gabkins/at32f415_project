#pragma once

#include "stdint.h"
#include "xprintf.h"
#include "IndicatorConfig.h"
#include <cstdarg> // Required for va_list, va_start, va_end
#include "stdio.h"

/**
 * @brief Шаблон абстрактного класса для Led-индикации.
 * Аппаратно-зависимые виртуальные методы 
 * virtual void Init();
 * virtual void Show();
 * virtual uint8_t GetCode(const char c);
 * нужно реализоваться в классе наследнике.
 * 
 * @tparam L Кол-во светодиодов 
 * @tparam D Конфигурация индикаторов 
 */
template <uint8_t L, uint8_t... D>
class _IndicationBase
{
private:

	/**
	 * @brief Константный массив, заполненный значениями из шаблонных параметров
	 * 
	 */
	static constexpr uint8_t group[] = {D...};

protected:
	/**
	 * @brief Константный метод, вычисляющий общее кол-во цифр на всех индикаторах(на этапе компилляции)
	 * 
	 * @return uint8_t constexpr 
	 */
	static uint8_t constexpr MaxDigits()
	{
		uint8_t res = 0;
		for (uint8_t i = 0; i < sizeof(group); i++)
			res += group[i];
		return res;
	}

public:

	/**
	 * @brief Какая-то структура для мигания. Нужно доделать. 
	 * 
	 */
protected:

	/**
	 * @brief Массив для хранения символов 
	 * 
	 */
	uint8_t digit[MaxDigits()];
	uint8_t blink[MaxDigits()];

	/**
	 * @brief Массив для хранения состояний светодиодов 
	 * 
	 */
	uint8_t led[L];

private:
	uint8_t *GetGroupPointer(uint8_t gp)
	{
		uint8_t index = 0;
		for (uint8_t i = 0; i < gp; i++)
			index += group[i];
		return &digit[index];
	}


	void SetDigits(const char *str, uint8_t *groupPointer, uint8_t groupSize)
	{
		for (uint8_t i = 0, k = 0; i < groupSize || (str[k] == '.'); i++, k++)
		{
			if (str[k] == '.')
			{
				if (i != 0)
				{
					i-=1;
					groupPointer[i] |= 0x80;
				}
			}
			else
			{
				groupPointer[i] = GetCode(str[k]);
			}
		}
	}

	uint8_t GetDigitIndex(uint8_t group_, uint8_t digit_)
	{
		uint8_t index = 0;
		for(uint8_t i = 0; i < group_; i++)
		{
			for(uint8_t j = 0; j < group[i]; j++)
			{
				index++;
			}
		}
		index += digit_;
		return index;
	}

	/**
	 * @brief Метод, возвращающий маску символа 
	 * 
	 * @param c ASCII код символа  
	 * @return uint8_t Маска символа для отображения
	 */
	virtual uint8_t GetCode(const char c) = 0;

	/**
	 * @brief Аппаратный метод отображения 
	 * 
	 */
	virtual void Show() = 0;

public:

	uint8_t GetGroupSize(uint8_t gp)
	{
		return group[gp];
	}
        
        uint8_t GetMaxPosition(uint8_t gp)
        {
                return group[gp] - 1;
        }
        
	/**
	 * @brief Аппаратная инициализация индикации
	 * 
	 */
	virtual void Init() = 0;

	/**
	 * @brief Вывод строки на нужный индикатор 
	 * 
	 * @param groupNum Номер индикатора, начиная с 0.
	 * @param string Выводимая строка
	 * @param ... 
	 */
	void Print(uint8_t groupNum, const char *string, ...)
	{
		//ClearGroup(groupNum);
		char buf[20];
		for(uint8_t i = 0; i < 20; i++) buf[i] = 0;
		va_list args;
		va_start(args, string);
		vxsprintf(buf, string, args);
		//sprintf(buf, string, args);
		va_end(args);
		SetDigits(buf, GetGroupPointer(groupNum), GetGroupSize(groupNum));
		ClearBlinksGroup(groupNum);
	}

	/**
	 * @brief Включает мигание одного символа. Автоматически выключается при вызове метода print в этой группе.
	 * 
	 * @param group_ номер группы
	 * @param digit_ номер символа
	 * @param c символ, которым происходит мигание
	 * @param blinkFlag 0 или 1. 0 - включить мигание, 1 - выключить
	 */
	void BlinkDigit(uint8_t group_, uint8_t digit_, const char c, uint8_t blinkFlag)
	{	
		if(blinkFlag == 0)
			blink[GetDigitIndex(group_, digit_)] = 0;
		else
			blink[GetDigitIndex(group_, digit_)] = c;
	}

	/**
	 * @brief Включает мигание группы символов. Автоматически выключается при вызове метода print в этой группе.
	 * 
	 * @param group_ номер группы
	 * @param c символ, которым происходит мигание
	 * @param blinkFlag 0 или 1. 0 - включить мигание, 1 - выключить
	 */
	void BlinkGroup(uint8_t group_, char* c, uint8_t blinkFlag)
	{	
		for(uint8_t i = 0; i < GetGroupSize(group_); i++)
			BlinkDigit(group_, i, c[i], blinkFlag);
	}

	/**
	 * @brief Включает мигание одного символа. Миганиме просиходит пробелом. Автоматически выключается при вызове метода print в этой группе.
	 * 
	 * @param group_ номер группы
	 * @param digit_ номер символа
	 * @param blinkFlag 0 или 1. 0 - включить мигание, 1 - выключить
	 */
	void BlinkDigit(uint8_t group_, uint8_t digit_, uint8_t blinkFlag)
	{	
		BlinkDigit(group_, digit_, ' ', blinkFlag);
	}

	/**
	 * @brief Выключить все мигалки на всех индикаторах
	 * 
	 */
	void ClearAllBlinks()
	{
		for(uint8_t i = 0; i < MaxDigits(); i++)
			blink[i] = 0;
	}


	/**
	 * @brief Выключить мигалки в определенной группе
	 * 
	 * @param group_ номер группы
	 */
	void ClearBlinksGroup(uint8_t group_)
	{
		uint8_t* groupStart = &blink[GetDigitIndex(group_, 0)];
		for(uint8_t i = 0; i < GetGroupSize(group_); i++)
			groupStart[i] = 0;
	}


	/**
	 * @brief Метод, задающий состояние нужного светодиода
	 * 
	 * @param num Номер светодиода, начиная с 0.
	 * @param state Состояние светодиода 0 или 1.
	 */
	void SetLed(uint8_t num, uint8_t state)
	{
		if (num < sizeof(led))
			led[num] = state;
	}

	/**
	 * @brief Очистка нужного индикатора
	 * 
	 * @param groupNum Номер индикатора, начиная с 0.
	 */
	void ClearGroup(uint8_t groupNum)
	{
		char buf[20];
		for (uint8_t i = 0; i < sizeof(buf); i++)
			buf[i] = ' ';
		SetDigits(buf, GetGroupPointer(groupNum), GetGroupSize(groupNum));
	}

	/**
	 * @brief Очистка нужной цифры
	 * 
	 * @param groupNum Номер индикатора, начиная с 0.
	 * @param digitNum Номер цифры на индикаторе, начиная с 0.
	 */
	void ClearDigit(uint8_t groupNum, uint8_t digitNum)
	{
		char c = ' ';
		SetDigits(&c, GetGroupPointer(groupNum) + digitNum, 1);
	}
	/**
	 * @brief Очистка всех индикаторов и светодиодов 
	 * 
	 */
	void ClearAll()
	{
		for (uint8_t i = 0; i < sizeof(led); i++)
			led[i] = 0;
		for (uint8_t i = 0; i < sizeof(group); i++)
			ClearGroup(i);
		ClearAllBlinks();
	}
	
	void blinkHandler()
	{
		#define PERIOD 500
		static uint32_t counter;
		counter++;
		if(counter > PERIOD) counter = 0;

		if(counter < PERIOD/2)
		{
			for(uint8_t i = 0; i < MaxDigits(); i++)
			{
				if(blink[i] != 0) digit[i] = GetCode(blink[i]);
			}
		}
	}

	/**
	* @brief Обработчик, который нужно поместить в прерывании по таймеру 1 мс.  
	* 
	*/
	void Handler() 
	{ 
		blinkHandler();
		Show();
	}
};

#ifndef INDICATOR_CONFIG
#error Требуется определить дефайн INDICATOR_CONFIG в файле "IndicatorConfig.h". \
Пример #define INDICATOR_CONFIG 8, 2, 4, 4  \
	(кол-во светодиодов, далее количество индикаторов в каждой группе слева направо и сверху вниз)
#endif
		
		typedef _IndicationBase<INDICATOR_CONFIG> IndicationBase;