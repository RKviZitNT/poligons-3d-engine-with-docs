#include "components/Camera.hpp"

// Конструктор с заданием направления и позиции
Camera::Camera(Vec3d direction, Vec3d position) : dir(direction), pos(position) {}

// Получение текущей позиции камеры
Vec3d Camera::getPos() { return pos; }

// Получение текущего направления камеры
Vec3d Camera::getDir() { update(); return dir; }

// Перемещение по оси X
void Camera::translateX(float offset) { pos.x += offset; }
// Перемещение по оси Y
void Camera::translateY(float offset) { pos.y += offset; }
// Перемещение по оси Z
void Camera::translateZ(float offset) { pos.z += offset; }

// Перемещение вперёд по направлению камеры
void Camera::translateForward(float offset) { pos += dir * offset; }
// Перемещение назад по направлению камеры
void Camera::translateBack(float offset) { pos -= dir * offset; }

// Перемещение вперёд без изменения Y
void Camera::translateForwardNoY(float offset) { pos += Vec3d(dir.x, 0, dir.z).normalize() * offset; }
// Перемещение назад без изменения Y
void Camera::translateBackNoY(float offset) { pos -= Vec3d(dir.x, 0, dir.z).normalize() * offset; }

// Перемещение влево относительно направления камеры
void Camera::translateLeft(float offset) { pos -= dir.cross({0, 1, 0}).normalize() * offset; }
// Перемещение вправо относительно направления камеры
void Camera::translateRight(float offset) { pos += dir.cross({0, 1, 0}).normalize() * offset; }

// Перемещение вверх (по оси Y)
void Camera::translateUp(float offset) { translateY(offset); }
// Перемещение вниз (по оси Y)
void Camera::translateDown(float offset) { translateY(-offset); }

// Вращение по горизонтали (изменение угла fYaw)
void Camera::rotateHorizontal(float offset) { fYaw += offset; }
// Вращение по вертикали (изменение угла fPitch)
void Camera::rotateVertical(float offset) { fPitch += offset; }

// Обновление направления камеры на основе углов fYaw и fPitch
void Camera::update() {
    // Максимальный угол наклона по вертикали
    const float maxPith = glbl::hpi;
    // Ограничение угла наклона сверху
    if (fPitch > maxPith) fPitch = maxPith;
    // Ограничение угла наклона снизу
    if (fPitch < -maxPith) fPitch = -maxPith;

    // Вычисление нового направления камеры
    dir = Vec3d(cosf(fYaw) * cosf(fPitch), sinf(fPitch), sinf(fYaw) * cosf(fPitch)).normalize();
}