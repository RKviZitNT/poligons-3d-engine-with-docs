#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <algorithm>

#include "Config.hpp"
#include "math/Vec2d.hpp"
#include "math/Vec3d.hpp"
#include "math/Mat4x4.hpp"
#include "components/props/Color.hpp"
#include "rendering/DepthBuffer.hpp"

// Класс для работы с треугольником в 3D-пространстве
class Triangle {
public:
    // Вершины треугольника
    std::array<Vec3d, 3> p;
    // Текстурные координаты для каждой вершины
    std::array<Vec2d, 3> t;  

    // Цвет треугольника
    Color col;
    // Освещённость треугольника
    float illumination;

    // Конструктор по умолчанию
    Triangle();
    // Конструктор с заданием вершин
    Triangle(Vec3d p1, Vec3d p2, Vec3d p3);

    // Установка цвета треугольника
    void setColor(Color color);
    // Установка текстурных координат
    void setTextureCoords(Vec2d t1, Vec2d t2, Vec2d t3);

    // Масштабирование треугольника для отображения на экране
    void scalingToDisplay();

    // Получение нормали треугольника
    Vec3d getNormal() const;

    // Проецирование треугольника (деление на w)
    void projectionDiv();

    // Умножение треугольника на матрицу трансформации
    Triangle operator*(const Mat4x4& mat);
    // Умножение треугольника на матрицу с присваиванием
    Triangle& operator*=(const Mat4x4& mat);

    // Отрисовка текстурированного треугольника
    sf::VertexArray texturedTriangle(DepthBuffer& depthBuffer, sf::Image* texture);

    // Отсечение треугольника относительно плоскости
    static int clipAgainsPlane(const Vec3d& planePoint, const Vec3d& planeNormal, const Triangle& inTri, Triangle& outTri1, Triangle& outTri2);

private:
    // Перемещение треугольника по оси X
    void translateX(float f);
    // Перемещение треугольника по оси Y
    void translateY(float f);
    // Перемещение треугольника по оси Z
    void translateZ(float f);

    // Масштабирование треугольника по оси X
    void scaleX(float f);
    // Масштабирование треугольника по оси Y
    void scaleY(float f);
    // Масштабирование треугольника по оси Z
    void scaleZ(float f);
};