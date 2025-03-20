#include "rendering/Render.hpp"

// Конструктор
Render::Render(Camera& camera) : m_camera(camera), m_depthBuffer(glbl::window::width, glbl::window::height) {}

// Добавление модели в список для рендеринга
void Render::addMesh(Mesh& mesh) {
    m_renderMeshes.emplace_back(&mesh);
}

// Обновление матриц вида и проекции
void Render::update() {
    // Матрица вида (инвертированная матрица "наведения" камеры)
    matView = Mat4x4::inverse(Mat4x4::pointAt(m_camera.getPos(), m_camera.getPos() + m_camera.getDir(), {0, 1, 0}));
    // Матрица проекции (перспективная проекция)
    matProj = Mat4x4::projection(glbl::render::fNear, glbl::render::fFar, glbl::render::fFov, (float)glbl::window::height / (float)glbl::window::width);
}

// Отрисовка сцены
void Render::render(sf::RenderWindow& window, Light light) {
    // Треугольники после проекции и отсечения
    std::vector<Triangle> projectedTriangles, renderedTriangles;
    // Текстурированные треугольники
    std::vector<sf::VertexArray> texturedTriangles;

    // Буферы для отрисовки треугольников и рёбер
    sf::VertexArray drawingTriangles(sf::PrimitiveType::Triangles);
    sf::VertexArray drawingEdges(sf::PrimitiveType::Lines);
    // Цвет рёбер
    sf::Color edgeColor(255, 128, 0);

    // Очистка буфера глубины
    m_depthBuffer.clear(0.f);

    // Обработка всех моделей
    for (auto& mesh : m_renderMeshes) {
        // Получение трансформированных треугольников
        std::vector<Triangle> triangles = mesh->getTransformedTriangles();

        // Обработка каждого треугольника
        for (auto& triangle : triangles) {
            // Проверка видимости задней грани (если включено)
            if (glbl::render::backFaceVisible || triangle.getNormal().dot(triangle.p[0] - m_camera.getPos()) < 0) {
                // Вычисление освещённости треугольника
                triangle.illumination = std::max(0.3f, triangle.getNormal().dot(light.getDir()));

                // Применение матрицы вида
                Triangle projectedTriangle = triangle;
                projectedTriangle *= matView;

                // Отсечение треугольника относительно ближней плоскости
                int clippedTriangles = 0;
                Triangle clipped[2];
                clippedTriangles = Triangle::clipAgainsPlane({0, 0, 0.1}, {0, 0, 1}, projectedTriangle, clipped[0], clipped[1]);
                for (size_t i = 0; i < clippedTriangles; i++) {
                    // Применение матрицы проекции
                    projectedTriangle = clipped[i] * matProj;

                    // Проецирование и масштабирование треугольника
                    projectedTriangle.projectionDiv();
                    projectedTriangle.scalingToDisplay();

                    // Добавление треугольника в список
                    projectedTriangles.emplace_back(projectedTriangle);
                }
            }
        }

        // Сортировка треугольников по глубине (если включён упрощённый рендеринг)
        if (glbl::render::liteRender) {
            std::sort(projectedTriangles.begin(), projectedTriangles.end(), [](const Triangle& t1, const Triangle& t2) {
                return (t1.p[0].z + t1.p[1].z + t1.p[2].z)/3 > (t2.p[0].z + t2.p[1].z + t2.p[2].z)/3;
            });
        }

        // Отсечение треугольников по границам экрана
        for (const auto& triangle : projectedTriangles) {
            Triangle clipped[2];
            std::deque<Triangle> triangles;
            triangles.emplace_back(triangle);
            int newTriangles = 1;

            // Отсечение по четырём границам экрана (верх, низ, лево, право)
            for (size_t i = 0; i < 4; i++) {
                int poligonsToAdd = 0;

                while (newTriangles > 0) {
                    Triangle tri = triangles.front();
                    triangles.pop_front();
                    newTriangles--;

                    switch (i) {
                    // Верхняя граница
                    case 0: poligonsToAdd = Triangle::clipAgainsPlane({0, 0, 0}, {0, 1, 0}, tri, clipped[0], clipped[1]); break;
                    // Нижняя граница
                    case 1: poligonsToAdd = Triangle::clipAgainsPlane({0, (float)glbl::window::height - 1, 0}, {0, -1, 0}, tri, clipped[0], clipped[1]); break;
                    // Левая граница
                    case 2: poligonsToAdd = Triangle::clipAgainsPlane({0, 0, 0}, {1, 0, 0}, tri, clipped[0], clipped[1]); break;
                    // Правая граница
                    case 3: poligonsToAdd = Triangle::clipAgainsPlane({(float)glbl::window::width - 1, 0, 0}, {-1, 0, 0}, tri, clipped[0], clipped[1]); break;
                    }

                    for (int j = 0; j < poligonsToAdd; j++) {
                        triangles.push_back(clipped[j]);
                    }
                }

                newTriangles = triangles.size();
            }

            // Добавление отсечённых треугольников в список
            for (const auto& tri : triangles) { renderedTriangles.emplace_back(tri); }
        }

        // Рендер текстурированных треугольников (если упрощённый рендеринг отключён)
        if (!glbl::render::liteRender) {
            for (auto& triangle : renderedTriangles) {
                texturedTriangles.emplace_back(triangle.texturedTriangle(m_depthBuffer, mesh->getTexture()));
            }
        }
    }

    // Отрисовка сцены
    if (glbl::render::liteRender) {
        // Упрощённый рендеринг (треугольники и рёбра)
        for (const auto& triangle : renderedTriangles) {
            sf::Color faceColor(triangle.col.r * triangle.illumination, triangle.col.g * triangle.illumination, triangle.col.b * triangle.illumination);

            // Отрисовка треугольников (если включено)
            if (glbl::render::faceVisible) {
                drawingTriangles.append(sf::Vertex{sf::Vector2f(triangle.p[0].x, triangle.p[0].y), faceColor});
                drawingTriangles.append(sf::Vertex{sf::Vector2f(triangle.p[1].x, triangle.p[1].y), faceColor});
                drawingTriangles.append(sf::Vertex{sf::Vector2f(triangle.p[2].x, triangle.p[2].y), faceColor});
            }

            // Отрисовка рёбер (если включено)
            if (glbl::render::edgeVisible) {
                drawingEdges.append(sf::Vertex{sf::Vector2f(triangle.p[0].x, triangle.p[0].y), edgeColor});
                drawingEdges.append(sf::Vertex{sf::Vector2f(triangle.p[1].x, triangle.p[1].y), edgeColor});
                drawingEdges.append(sf::Vertex{sf::Vector2f(triangle.p[1].x, triangle.p[1].y), edgeColor});
                drawingEdges.append(sf::Vertex{sf::Vector2f(triangle.p[2].x, triangle.p[2].y), edgeColor});
                drawingEdges.append(sf::Vertex{sf::Vector2f(triangle.p[2].x, triangle.p[2].y), edgeColor});
                drawingEdges.append(sf::Vertex{sf::Vector2f(triangle.p[0].x, triangle.p[0].y), edgeColor});
            }
        }

        // Отрисовка треугольников и рёбер на экран
        if (glbl::render::faceVisible && drawingTriangles.getVertexCount() > 0) {
            window.draw(drawingTriangles);
        }

        if (glbl::render::edgeVisible && drawingEdges.getVertexCount() > 0) {
            window.draw(drawingEdges);
        }
    }
    else {
        // Отрисовка текстурированных треугольников
        for (const auto& pixels : texturedTriangles) {
            window.draw(pixels);
        }
    }
}