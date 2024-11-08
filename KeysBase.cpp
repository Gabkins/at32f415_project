#include "KeysBase.h"

/**
 * @brief Конструктор класса KeysBase
 *
 * @param keysConfig - массив с конфигурацией кнопок, ограниченный байтом 0.
 * @param keysNumber - Кол-во эл-ов массива = кол-ву кнопок.
 * Элемент = доступная функция: долгое нажатие, автоповтор или 0. Пример:
 *  const uint8_t keysConfig[] =
 *  {
 *      Keys::LONG_OPTION,
 *      Keys::LONG_OPTION,
 *      Keys::REPEAT_OPTION,
 *      Keys::REPEAT_OPTION,
        0
 *  } ;
 *  const uint8_t keysNumber = 4;
 */
KeysBase::KeysBase(uint8_t const* keysConfig, uint8_t keysNumber)
{
    config = keysConfig;
    number = keysNumber;
    for ( uint8_t i = 0; i < number; i++ )
    {
        if(config[i] & LONG_OPTION)   longpressMask |= (1 << i);
        if(config[i] & REPEAT_OPTION) repeatMask    |= (1 << i);
    }
}

/**
 * @brief Метод, возвращающий событие от кнопок.
 *
 * @return KeysBase::Event
 */
KeysBase::Event KeysBase::GetEvent()
{
  if(!buffer.IsEmpty()){
    auto r = buffer.Front();
    buffer.Pop();
    return r;
  }
  return Event();
}

/**
 * @brief Обработчик, который нужно поместить в прерывании по таймеру 1 мс.
 *
 */
void KeysBase::Handler()
{
    if ( ! (clock & 0x0001) ) // 2.5 mS (2mS)
    {
        uint8_t event    = 0,
                keysMask = Read();

        if ( keysMask != newState )
        {
            newState  = keysMask;
            bounceCount = BOUNCE_TIME; // 25 mS
        }
        keysMask = lastState;

        if ( (--repeatCount == 0) && (keysMask & repeatMask) )
        {
            event = REPEAT;
            repeatCount = repeatDelay;
            if(repeatDelay > MIN_REPEAT_DELAY) repeatDelay -= 3;
            else
            {
                repeatDelay = (MAX_REPEAT_DELAY - 32);
                event = FAST_REPEAT;
            }
        }

        if ( bounceCount ) bounceCount--;
        else if ( keysMask != newState )
        {
            if(newState > keysMask) event = PRESSED;
            longpressCount = 0;
            repeatCount = DELAY_BEFORE_REPEAT;
            repeatDelay = MAX_REPEAT_DELAY;
            keysMask = newState;
        }

        if ( event ) timeoutKeyCount = 0;
        if ( uint8_t(clock) == 0 ) // 320 mS = (1.25 mS x 256)
        {
            if ( (keysMask & longpressMask) && ++longpressCount > LONGPRESS_DELAY )
            {
                longpressCount = 0;
                event = LONGPRESS;
                timeoutKeyCount = 0;
            }
            else if ( ++timeoutKeyCount > UST_TIME )
            {
                timeoutKeyCount = 0;
                event = TIMEOUT;
            }
        }

        if ( event ) 
          SetEvent ( Event{event, keysMask} ) ;
        lastState = keysMask;
    }
    clock++;
}

/**
 * @brief Приватный метод, помещающий событие event в буфер FIFO.
 *
 * @param event
 */
void KeysBase::SetEvent(Event event)
{
    buffer.Push(event);
}
