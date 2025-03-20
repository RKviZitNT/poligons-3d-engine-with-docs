#pragma once

#include <memory>
#include <stdexcept>
#include <algorithm>

// Класс для работы с буфером глубины (Z-буфер)
class DepthBuffer {
public:
    // Конструктор по умолчанию
    DepthBuffer() = default;
    // Конструктор с заданием размеров
    DepthBuffer(int width, int height);

    // Запрет копирования
    DepthBuffer(const DepthBuffer&) = delete;
    DepthBuffer& operator=(const DepthBuffer&) = delete;

    // Разрешение перемещения
    DepthBuffer(DepthBuffer&&) = default;
    DepthBuffer& operator=(DepthBuffer&&) = default;

    // Изменение размера буфера
    void resize(int width, int height);

    // Очистка буфера (заполнение значением по умолчанию 1.0)
    void clear(float value = 1.0f) noexcept;

    // Доступ к элементам буфера по индексу
    float& operator()(int index);
    const float& operator()(int index) const;

    // Получение размеров буфера
    int width() const noexcept { return m_width; }
    int height() const noexcept { return m_height; }

private:
// Динамический массив для хранения значений глубины
    std::unique_ptr<float[]> m_depthBuffer;
    // Ширина буфера
    int m_width = 0;
    // Высота буфера
    int m_height = 0;

    // Валидация размеров буфера
    void validateDimensions(int width, int height) const;

    // Валидация индекса
    void validateCoordinates(int index) const;
};