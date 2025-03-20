#pragma once

// Класс для представления цвета в формате RGB
class Color {
public:
    // Компоненты цвета (красный, зелёный, синий), по умолчанию белый цвет
    float r = 255, g = 255, b = 255;

    // Конструктор по умолчанию (белый цвет)
    Color() = default;
    // Конструктор для оттенков серого (r = g = b = gray)
    Color(float gray);
    // Конструктор с отдельными значениями для r, g, b
    Color(float r_, float g_, float b_);

    // Перегрузка оператора для умножения на яркость
    Color operator*(float brightness);
};