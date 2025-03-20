#pragma once

// Класс для работы с 2D-вектором (u, v) и дополнительной компонентой w (для проекций)
class Vec2d {
public:
    // 2D-координаты
    float u = 0, v = 0;
    // Компонента для проекции (по умолчанию 1)
    float w = 1;

    // Конструктор по умолчанию
    Vec2d() = default;
    // Конструктор с одинаковыми значениями для u и v
    Vec2d(float uv);
    // Конструктор с отдельными значениями для u и v
    Vec2d(float _u, float _v);

   // Перегрузка стандартных операторов
    Vec2d operator+(const Vec2d& other) const;
    Vec2d operator-(const Vec2d& other) const;
    Vec2d operator*(const Vec2d& other) const;
    Vec2d operator/(const Vec2d& other) const;

    Vec2d operator+(const float& f) const;
    Vec2d operator-(const float& f) const;
    Vec2d operator*(const float& f) const;
    Vec2d operator/(const float& f) const;

    // Унарный минус (инверсия знака)
    Vec2d operator-() const;

    // Перегрузка опрераторов присваивания
    Vec2d& operator+=(const Vec2d& other);
    Vec2d& operator-=(const Vec2d& other);
    Vec2d& operator*=(const Vec2d& other);
    Vec2d& operator/=(const Vec2d& other);

    Vec2d& operator+=(const float& f);
    Vec2d& operator-=(const float& f);
    Vec2d& operator*=(const float& f);
    Vec2d& operator/=(const float& f);

    // Деление u и v на w (для проекции)
    void projectionDiv(float w_);
    // Установка значения w
    void setW(float w_);

    // Вычисление ересечения линии с плоскостью
    void intersectPlane(const Vec2d& lineStart, const Vec2d& lineEnd, float& t);
};