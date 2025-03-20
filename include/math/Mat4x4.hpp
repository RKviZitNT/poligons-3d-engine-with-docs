#pragma once

#include <initializer_list>
#include <array>
#include <cmath>

#include "Config.hpp"

// Даём знать программе, что существует класс Vec3d (для использования в методах)
class Vec3d;

// Класс для работы с матрицей 4x4 (используется для трансформаций и проекций)
class Mat4x4 {
public:
    // Матрица 4x4 (двумерный массив)
    std::array<std::array<float, 4>, 4> m;

    // Конструктор по умолчанию
    Mat4x4() = default;
    // Инициализация списком значений
    Mat4x4(std::initializer_list<std::initializer_list<float>> values);

    // Перегрузка оператора умножения матриц
    Mat4x4 operator*(const Mat4x4& other) const;

    // Матрица перемещения
    static Mat4x4 translation(float x, float y, float z);
    // Матрица масштабирования
    static Mat4x4 scale(float scaleX, float scaleY, float scaleZ);
    // Матрица вращения вокруг оси X
    static Mat4x4 rotationX(float angle);
    // Матрица вращения вокруг оси Y
    static Mat4x4 rotationY(float angle);
    // Матрица вращения вокруг оси Z
    static Mat4x4 rotationZ(float angle);
    // Матрица проекции
    static Mat4x4 projection(float fNear, float fFar, float fFov, float fAspectRatio);
    // Матрица "наведения"
    static Mat4x4 pointAt(const Vec3d& pos, const Vec3d& target, const Vec3d& up);
    // Обратная матрица
    static Mat4x4 inverse(const Mat4x4& m);
};