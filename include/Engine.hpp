#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Config.hpp"
#include "math/Vec3d.hpp"
#include "components/geometry/Mesh.hpp"
#include "components/lightning/Light.hpp"
#include "rendering/Render.hpp"

// Класс для управления основным циклом приложения (игровым движком)
class Engine {
public:
     // Конструктор (инициализация окна, камеры, света и модели)
    Engine();

    // Основной цикл приложения
    void run();

private:
    // Окно приложения
    sf::RenderWindow m_window;
    // Время, прошедшее с последнего кадра
    sf::Time deltaTime;

    // Центр окна (для управления камерой мышью)
    sf::Vector2i windowCenter;
    // Флаг, указывающий, заблокирован ли курсор мыши
    bool m_isMouseLocked;
    // Флаг, указывающий, находится ли приложение в режиме паузы
    bool m_isPaused;

    // Рендер (отвечает за отрисовку сцены)
    Render m_render;
    // Камера (управление видом сцены)
    Camera m_camera;
    // Источник света
    Light m_light;

    // 3D-модель
    Mesh m_cube;

    // Скорость перемещения и вращения камеры
    float m_cameraTranslateSpeed, m_cameraRotateSpeed;

    // Обработка событий (ввод пользователя)
    void handleEvents();
    // Обновление состояния игры
    void update();
    // Отрисовка сцены
    void draw();
};