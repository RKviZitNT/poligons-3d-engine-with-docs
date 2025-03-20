#include "rendering/DepthBuffer.hpp"

// Конструктор с заданием размеров
DepthBuffer::DepthBuffer(int width, int height) { resize(width, height); }

// Изменение размера буфера
void DepthBuffer::resize(int width, int height) {
    // Проверка корректности размеров
    validateDimensions(width, height);

    // Если размеры не изменились, выходим
    if (width == m_width && height == m_height) return;

    // Создание нового буфера
    m_depthBuffer = std::make_unique<float[]>(width * height);
    // Обновление ширины
    m_width = width;
    // Обновление высоты
    m_height = height;

    // Очистка буфера
    clear(0.f);
}

// Очистка буфера
void DepthBuffer::clear(float value) noexcept {
    // Если буфер существует
    if (m_depthBuffer) {
        // Заполнение значением
        std::fill(m_depthBuffer.get(), m_depthBuffer.get() + m_width * m_height, value);
    }
}

// Доступ к элементу буфера по индексу (неконстантная версия)
float& DepthBuffer::operator()(int index) {
    // Проверка корректности индекса
    validateCoordinates(index);
    return m_depthBuffer[index];
}

// Доступ к элементу буфера по индексу (константная версия)
const float& DepthBuffer::operator()(int index) const {
    // Проверка корректности индекса
    validateCoordinates(index);
    return m_depthBuffer[index];
}

// Валидация размеров буфера
void DepthBuffer::validateDimensions(int width, int height) const {
    // Ошибка, если размеры некорректны
    if (width <= 0 || height <= 0) { throw std::invalid_argument("Dimensions must be positive"); }
}

// Валидация индекса
void DepthBuffer::validateCoordinates(int index) const {
    // Ошибка, если индекс некорректен
    if (index > m_width * m_height || index < 0) { throw std::out_of_range("Invalid coordinates"); }
}