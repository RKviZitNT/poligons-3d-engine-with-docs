#include "components/lightning/Light.hpp"

// Конструктор с заданием направления света
Light::Light(Vec3d direction) {
    // Устанавливаем направление света
    setDir(direction);
}

// Установка направления света
void Light::setDir(Vec3d direction) {
    // Инвертируем x и нормализуем вектор
    dir = Vec3d(-direction.x, direction.y, direction.z).normalize();
}

// Получение текущего направления света
Vec3d Light::getDir() { return dir; }