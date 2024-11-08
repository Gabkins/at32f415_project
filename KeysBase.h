#pragma once

#include "stdint.h"
#include "StaticQueue.h" // https://skgit.ddns.net/Ivan/ledlibraries/-/blob/dev/StaticQueue/StaticQueue.h

/**
 * @brief Базовый абстрактный класс для кнопок.
 * Аппаратно-зависимые виртуальные методы
 * virtual void Init();
 * virtual uint8_t Read();
 * нужно реализовать в классе наследнике.
 */
class KeysBase
{
    public:

        /**
         * @brief Структура для событий от кнопок, содержащая тип события и значение.
         * Возможные типы событий:
         * PRESSED - обычное нажатие,
         * LONGPRESS - долгое нажатие,
         * REPEAT - автоповтор(при длительном нажатии),
         * FAST_REPEAT - событие, возникающее после автоповтора.
         * Значение содержит информацию, какая нажата кнопка.
         */
        struct Event
        {
            uint8_t type,
                    value;
        } event{0,0} ;

        /**
         * @brief Размер FIFO-буфера событий.
         *
         */
        static constexpr uint8_t BUFFER_SIZE = 8;

    private:

        StaticQueue < Event, BUFFER_SIZE> buffer;

    protected:

        /**
         * @brief Указатель на массив с конфигурацией кнопок и количество кнопок.
         *
         */
        uint8_t const* config = nullptr;
        uint8_t        number = 0;

    public:

        KeysBase(uint8_t const* keysConfig, uint8_t keysNumber);

        /**
         * @brief Аппаратная инициализация кнопок.
         *
         */
        virtual void Init() = 0;

        Event GetEvent();

        void Handler();

    private:

        void SetEvent(Event event);
        /**
         * @brief Аппаратное чтение кнопок.
         *
         * @return uint8_t
         */
        virtual uint8_t Read() = 0;

    protected:

        uint8_t repeatMask    = 0,
                longpressMask = 0;

    private:

        uint16_t clock = 0;

        uint8_t bounceCount     = 0,
                timeoutKeyCount = 0,
                lastState       = 0,
                newState        = 0,
                repeatCount     = 0,
                repeatDelay     = 0,
                longpressCount  = 0;

    public:

        static constexpr uint8_t KEYS = 0x20,

                                 PRESSED     = KEYS,
                                 LONGPRESS   = KEYS|0x01,
                                 REPEAT      = KEYS|0x02,
                                 FAST_REPEAT = KEYS|0x04,
                                 TIMEOUT     = KEYS|0x08,

                                 LONG_OPTION   = 1/* bit mask */,
                                 REPEAT_OPTION = 2/* bit mask */;

    private:

        static constexpr uint8_t BOUNCE_TIME         = 13,
                                 UST_TIME            = 233,
                                 DELAY_BEFORE_REPEAT = 250,
                                 MAX_REPEAT_DELAY    = 176,
                                 MIN_REPEAT_DELAY    = 16,
                                 LONGPRESS_DELAY     = 38;
} ;
