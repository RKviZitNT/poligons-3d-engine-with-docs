#include "components/props/Color.hpp"

// Конструктор для оттенков серого
Color::Color(float gray) : r(gray), g(gray), b(gray) {}

// Конструктор с отдельными значениями для r, g, b
Color::Color(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}

// Перегрузка оператора умножения на яркость
Color Color::operator*(float brightness) {
    // Проверка, что яркость в допустимом диапазоне [0, 1]
    if (brightness >= 0 && brightness <= 1) {
        return { r * brightness, g * brightness, b * brightness };
    }
    // Если яркость вне диапазона, возвращаем исходный цвет
    return { r, g, b };
}