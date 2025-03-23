#pragma once

#include <iostream>

namespace glbl {
    // Константа числа ПИ
    constexpr float pi = 3.14159265f;
    // Половина чиста ПИ
    constexpr float hpi = pi / 2.f - 0.00001f;
    // Радиан
    constexpr float rad = pi / 180.f;

    namespace window {
        // Ширина окна
        constexpr int width = 1920;
        // Высота окна
        constexpr int height = 1080;

        // Частота кадров
        constexpr int frameRate = 60;
    }

    namespace render {
        // Ближняя плоскость проекции
        constexpr float fNear = 0.1f;
        // Дальняя плоскость проекции
        constexpr float fFar = 1000.f;
        // Угол обзора
        constexpr float fFov = 80.f;

        // Отображение текстур
        constexpr bool textureVisible = true;
        // Отображение "отвёрнутых" треугольников
        constexpr bool backFaceVisible = false;
        // Отображение треугольников (только в упрощённом рендере)
        constexpr bool faceVisible = true;
        // Отображение граней (только в упрощённом рендере)
        constexpr bool edgeVisible = false;

        // Упрощённый рендер
        constexpr bool liteRender = false;
    }

    // Функция для дебага
    inline void debug() {
        std::cout << std::endl;
    }

    template<typename T, typename... Args>
    inline void debug(T first, Args... args) {
        std::cout << first;
        if constexpr (sizeof...(args) > 0) {
            std::cout << ", ";
        }
        debug(args...);
    }
}