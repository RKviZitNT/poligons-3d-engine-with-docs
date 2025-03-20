#pragma once

#include <cmath>

#include "Config.hpp"
#include "math/Vec3d.hpp"

// Класс для управления камерой в 3D-пространстве
class Camera {
public:
    // Конструктор по умолчанию
    Camera() = default;
    // Конструктор с заданием направления и позиции
    Camera(Vec3d direction, Vec3d position);

    // Возвращает текущую позицию камеры
    Vec3d getPos();
    // Возвращает текущее направление камеры
    Vec3d getDir();

    // Перемещение по оси X
    void translateX(float offset);
    // Перемещение по оси Y
    void translateY(float offset);
    // Перемещение по оси Z
    void translateZ(float offset);

    // Перемещение вперёд по направлению камеры
    void translateForward(float offset);
    // Перемещение назад по направлению камеры  
    void translateBack(float offset);

    // Перемещение вперёд без изменения Y
    void translateForwardNoY(float offset);
    // Перемещение назад без изменения Y
    void translateBackNoY(float offset);

    // Перемещение влево относительно направления камеры
    void translateLeft(float offset);
    // Перемещение вправо относительно направления камеры
    void translateRight(float offset);

    // Перемещение вверх (по оси Y)
    void translateUp(float offset);
    // Перемещение вниз (по оси Y)
    void translateDown(float offset);

    // Вращение по горизонтали (изменение угла fYaw)
    void rotateHorizontal(float offset);
    // Вращение по вертикали (изменение угла fPitch)
    void rotateVertical(float offset);

private:
    // Позиция и направление камеры
    Vec3d pos = Vec3d(0), dir = Vec3d(0, 0, 1);
    // Угол поворота по горизонтали (в радианах)
    float fYaw = glbl::hpi;
    // Угол поворота по вертикали (в радианах)
    float fPitch = 0;

    // Обновление направления камеры на основе углов fYaw и fPitch
    void update();
};