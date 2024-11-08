#pragma once

#include <cstdint>
#include "stddef.h"

template <typename T, size_t MaxSize>
class StaticQueue
{
private:
    T data[MaxSize];
    size_t front;
    size_t rear;
    size_t count;

public:
    StaticQueue() : front(0), rear(0), count(0) { }

    /**
     * @brief Добавляет элемент в очередь. Если очередь полна, перезаписывает
     *        самый старый элемент.
     *
     * @param value Элемент для добавления
     */
    void Push(const T& value)
    {
        if (count == MaxSize)
            front = (front + 1) % MaxSize;  // Перезаписываем элемент, если очередь полна
        else
            ++count;

        data[rear] = value;
        rear = (rear + 1) % MaxSize;
    }

    /**
     * @brief Удаляет элемент из очереди.
     *        Если очередь пуста, возвращает значение по умолчанию.
     *
     * @return Удалённый элемент
     */
    T& Pop()
    {
        T* result = (T*)(&data[front]);
        front = (front + 1) % MaxSize;
        --count;
        return *result;
    }

    /**
     * @brief Возвращает первый элемент очереди.
     *        Если очередь пуста, возвращает значение по умолчанию.
     *
     * @return Первый элемент
     */
    T& Front() const
    {
        return (T&)data[front];
    }

    /**
     * @brief Возвращает последний элемент очереди.
     *        Если очередь пуста, возвращает значение по умолчанию.
     *
     * @return Последний элемент
     */
    T& Back() const
    {
        return (T&)data[(rear + MaxSize - 1) % MaxSize];
    }

    /**
     * @brief Проверяет, пуста ли очередь.
     *
     * @return true если очередь пуста, иначе false
     */
    bool IsEmpty() const
    {
        return count == 0;
    }

    /**
     * @brief Проверяет, полна ли очередь.
     *
     * @return true если очередь полна, иначе false
     */
    bool IsFull() const
    {
        return count == MaxSize;
    }

    /**
     * @brief Возвращает количество элементов в очереди.
     *
     * @return Количество элементов
     */
    size_t GetSize() const
    {
        return count;
    }
};
