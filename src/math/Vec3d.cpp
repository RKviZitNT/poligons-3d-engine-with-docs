#include "math/Vec3d.hpp"

// Конструктор с одинаковыми значениями для x, y, z
Vec3d::Vec3d(float xyz) : x(xyz), y(xyz), z(xyz) {}
// Конструктор с отдельными значениями для x, y, z
Vec3d::Vec3d(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

// Перегрузка стандартных операторов
Vec3d Vec3d::operator+(const Vec3d& other) const { return { x + other.x, y + other.y, z + other.z }; }
Vec3d Vec3d::operator-(const Vec3d& other) const { return { x - other.x, y - other.y, z - other.z }; }
Vec3d Vec3d::operator*(const Vec3d& other) const { return { x * other.x, y * other.y, z * other.z }; }
Vec3d Vec3d::operator/(const Vec3d& other) const { return { x / other.x, y / other.y, z / other.z }; }

Vec3d Vec3d::operator+(const float& f) const { return { x + f, y + f, z + f }; }
Vec3d Vec3d::operator-(const float& f) const { return { x - f, y - f, z - f }; }
Vec3d Vec3d::operator*(const float& f) const { return { x * f, y * f, z * f }; }
Vec3d Vec3d::operator/(const float& f) const { return { x / f, y / f, z / f }; }

// Унарный минус (инверсия знака)
Vec3d Vec3d::operator-() const { return { -x, -y, -z }; }

// Перегрузка опрераторов присваивания
Vec3d& Vec3d::operator+=(const Vec3d& other) { x += other.x; y += other.y; z += other.z; return *this; }
Vec3d& Vec3d::operator-=(const Vec3d& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
Vec3d& Vec3d::operator*=(const Vec3d& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
Vec3d& Vec3d::operator/=(const Vec3d& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }

Vec3d& Vec3d::operator+=(const float& f) { x += f; y += f; z += f; return *this; }
Vec3d& Vec3d::operator-=(const float& f) { x -= f; y -= f; z -= f; return *this; }
Vec3d& Vec3d::operator*=(const float& f) { x *= f; y *= f; z *= f; return *this; }
Vec3d& Vec3d::operator/=(const float& f) { x /= f; y /= f; z /= f; return *this; }

// Умножение вектора на матрицу 4x4
Vec3d Vec3d::operator*(const Mat4x4& mat) {
    Vec3d result;
    result.x = x * mat.m[0][0] + y * mat.m[1][0] + z * mat.m[2][0] + w * mat.m[3][0];
    result.y = x * mat.m[0][1] + y * mat.m[1][1] + z * mat.m[2][1] + w * mat.m[3][1];
    result.z = x * mat.m[0][2] + y * mat.m[1][2] + z * mat.m[2][2] + w * mat.m[3][2];
    result.w = x * mat.m[0][3] + y * mat.m[1][3] + z * mat.m[2][3] + w * mat.m[3][3];
    return result;
}

// Векторное произведение
Vec3d Vec3d::cross(const Vec3d& other) const {
    return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
}

// Скалярное произведение
float Vec3d::dot(const Vec3d& other) const { return x * other.x + y * other.y + z * other.z; }

// Вычисление длины вектора
float Vec3d::length() const { return sqrtf(x * x + y * y + z * z); }

// Нормализация вектора
Vec3d Vec3d::normalize() const {
    // Вычисление длины
    float len = length();
    if (len > 0) {
        // Возврат нормализованного вектора
        return { x / len, y / len, z / len };
    }
    // Если длина нулевая, возвращаем нулевой вектор
    return Vec3d(0);
}

// Деление x, y, z на w (для проекции)
void Vec3d::projectionDiv() { x /= w; y /= w; z /= w; }

// Пересечение линии с плоскостью
void Vec3d::intersectPlane(const Vec3d& planePoint, const Vec3d& planeNormal, const Vec3d& lineStart, const Vec3d& lineEnd, float& t) {
    // Нормализация нормали плоскости
    Vec3d normalizedPlaneNormal = planeNormal.normalize();
    // Вычисление расстояния до плоскости
    float plane_d = -normalizedPlaneNormal.dot(planePoint);

    // Скалярное произведение начала линии и нормали
    float ad = lineStart.dot(normalizedPlaneNormal);
    // Скалярное произведение конца линии и нормали
    float bd = lineEnd.dot(normalizedPlaneNormal);

    // Вычисление параметра t для точки пересечения
    t = (-plane_d - ad) / (bd - ad);

    // Вектор от начала до конца линии
    Vec3d lineStartToEnd = lineEnd - lineStart;
    // Вектор до точки пересечения
    Vec3d lineToIntersect = lineStartToEnd * t;
    // Результат — точка пересечения
    *this = lineStart + lineToIntersect;
}