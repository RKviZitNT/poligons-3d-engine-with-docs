#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>

#include "Config.hpp"
#include "math/Mat4x4.hpp"
#include "math/Vec3d.hpp"
#include "components/geometry/Triangle.hpp"
#include "components/Camera.hpp"
#include "components/geometry/Mesh.hpp"
#include "components/lightning/Light.hpp"
#include "rendering/DepthBuffer.hpp"

// Класс для рендеринга 3D-сцены
class Render {
public:
// Конструктор (принимает камеру)
    Render(Camera& camera);

    // Добавление модели в список для рендеринга
    void addMesh(Mesh& mesh);

    // Обновление матриц вида и проекции
    void update();
    // Отрисовка сцены
    void render(sf::RenderWindow& window, Light light);

private:
    // Список моделей для рендеринга
    std::vector<Mesh*> m_renderMeshes;
    // Камера (для вычисления матриц вида и проекции)
    Camera& m_camera;

    // Матрицы вида и проекции
    Mat4x4 matView, matProj;

    // Буфер глубины для корректного отображения перекрытий
    DepthBuffer m_depthBuffer;
};