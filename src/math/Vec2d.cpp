#include "math/Vec2d.hpp"

// Инициализация u и v одинаковым значением
Vec2d::Vec2d(float uv) : u(uv), v(uv) {}
// Инициализация u и v отдельными значениями
Vec2d::Vec2d(float _u, float _v) : u(_u), v(_v) {}

// Перегрузка операторов для работы с векторами
Vec2d Vec2d::operator+(const Vec2d& other) const { return { u + other.u, v + other.v }; }
Vec2d Vec2d::operator-(const Vec2d& other) const { return { u - other.u, v - other.v }; }
Vec2d Vec2d::operator*(const Vec2d& other) const { return { u * other.u, v * other.v }; }
Vec2d Vec2d::operator/(const Vec2d& other) const { return { u / other.u, v / other.v }; }

// Перегрузка операторов для работы с вектором и скаляром
Vec2d Vec2d::operator+(const float& f) const { return { u + f, v + f }; }
Vec2d Vec2d::operator-(const float& f) const { return { u - f, v - f }; }
Vec2d Vec2d::operator*(const float& f) const { return { u * f, v * f }; }
Vec2d Vec2d::operator/(const float& f) const { return { u / f, v / f }; }

// Унарный минус (инверсия знака)
Vec2d Vec2d::operator-() const { return { -u, -v }; }

// Операторы присваивания с операцией
Vec2d& Vec2d::operator+=(const Vec2d& other) { u += other.u; v += other.v; return *this; }
Vec2d& Vec2d::operator-=(const Vec2d& other) { u -= other.u; v -= other.v; return *this; }
Vec2d& Vec2d::operator*=(const Vec2d& other) { u *= other.u; v *= other.v; return *this; }
Vec2d& Vec2d::operator/=(const Vec2d& other) { u /= other.u; v /= other.v; return *this; }

Vec2d& Vec2d::operator+=(const float& f) { u += f; v += f; return *this; }
Vec2d& Vec2d::operator-=(const float& f) { u -= f; v -= f; return *this; }
Vec2d& Vec2d::operator*=(const float& f) { u *= f; v *= f; return *this; }
Vec2d& Vec2d::operator/=(const float& f) { u /= f; v /= f; return *this; }

// Деление u и v на w (для проекции)
void Vec2d::projectionDiv(float w_) { u /= w_; v /= w_; }
// Установка значения w
void Vec2d::setW(float w_) { w = w_; }

// Метод для нахождения пересечения линии с плоскостью
void Vec2d::intersectPlane(const Vec2d& lineStart, const Vec2d& lineEnd, float& t) {
    // Интерполяция u
    u = lineStart.u + t * (lineEnd.u - lineStart.u);
    // Интерполяция v 
    v = lineStart.v + t * (lineEnd.v - lineStart.v);
    // Интерполяция w
    w = lineStart.w + t * (lineEnd.w - lineStart.w);
}