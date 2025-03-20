#pragma once

#include <cmath>

#include "Mat4x4.hpp"

// Класс для работы с 3D-вектором (x, y, z) и дополнительной компонентой w (для однородных координат)
class Vec3d {
public:
// 3D-координаты
    float x = 0, y = 0, z = 0;
    // Компонента для проекции (по умолчанию 1)
    float w = 1;

    // Конструктор по умолчанию
    Vec3d() = default;
    // Конструктор с одинаковыми значениями для x, y, z
    Vec3d(float xyz);
    // Конструктор с отдельными значениями для x, y, z
    Vec3d(float _x, float _y, float _z);

    // Перегрузка стандартных операторов
    Vec3d operator+(const Vec3d& other) const;
    Vec3d operator-(const Vec3d& other) const;
    Vec3d operator*(const Vec3d& other) const;
    Vec3d operator/(const Vec3d& other) const;

    Vec3d operator+(const float& f) const;
    Vec3d operator-(const float& f) const;
    Vec3d operator*(const float& f) const;
    Vec3d operator/(const float& f) const;

    // Унарный минус (инверсия знака)
    Vec3d operator-() const;

    // Перегрузка опрераторов присваивания
    Vec3d& operator+=(const Vec3d& other);
    Vec3d& operator-=(const Vec3d& other);
    Vec3d& operator*=(const Vec3d& other);
    Vec3d& operator/=(const Vec3d& other);

    Vec3d& operator+=(const float& f);
    Vec3d& operator-=(const float& f);
    Vec3d& operator*=(const float& f);
    Vec3d& operator/=(const float& f);

    // Умножение вектора на матрицу 4x4
    Vec3d operator*(const Mat4x4& mat);

    // Векторное произведение
    Vec3d cross(const Vec3d& other) const;  
    // Скалярное произведение
    float dot(const Vec3d& other) const;

    // Вычисление длины вектора
    float length() const;
     // Нормализация вектора (приведение к длине 1)
    Vec3d normalize() const;

    // Деление x, y, z на w (для проекции)
    void projectionDiv();

    // Вычисление ересечения линии с плоскостью
    void intersectPlane(const Vec3d& planePoint, const Vec3d& planeNormal, const Vec3d& lineStart, const Vec3d& lineEnd, float& t);
};