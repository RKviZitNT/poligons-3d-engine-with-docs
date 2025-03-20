#pragma once

#include "math/Vec3d.hpp"

// Класс для представления источника света (направленный свет)
class Light {
public:
    // Конструктор по умолчанию
    Light() = default;
    // Конструктор с заданием направления света
    Light(Vec3d direction);

    // Установка направления света
    void setDir(Vec3d direction);
    // Получение текущего направления света
    Vec3d getDir();

private:
    // Направление света
    Vec3d dir;
};