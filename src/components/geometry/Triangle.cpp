#include "components/geometry/Triangle.hpp"

// Конструктор по умолчанию
Triangle::Triangle() : p{ Vec3d(), Vec3d(), Vec3d() } {}

// Конструктор с заданными вершинами
Triangle::Triangle(Vec3d p1, Vec3d p2, Vec3d p3) : p{ p1, p2, p3 } {}

// Установка цвета треугольника
void Triangle::setColor(Color color) { col = color; }

// Установка текстурных координат
void Triangle::setTextureCoords(Vec2d t1, Vec2d t2, Vec2d t3) {
    t[0] = t1;
    t[1] = t2;
    t[2] = t3;
}

// Масштабирование треугольника для отображения на экране
void Triangle::scalingToDisplay() {
    // Инверсия по оси X
    scaleX(-1);
    // Инверсия по оси Y
    scaleY(-1);
    // Перемещение по оси X
    translateX(1.f);
    // Перемещение по оси Y
    translateY(1.f);
    // Масштабирование по оси X
    scaleX(0.5f * glbl::window::width);
    // Масштабирование по оси Y
    scaleY(0.5f * glbl::window::height);
}

// Перемещение треугольника по оси X
void Triangle::translateX(float f) { for (auto& v : p) { v.x += f; } }

// Перемещение треугольника по оси Y
void Triangle::translateY(float f) { for (auto& v : p) { v.y += f; } }

// Перемещение треугольника по оси Z
void Triangle::translateZ(float f) { for (auto& v : p) { v.z += f; } }

// Масштабирование треугольника по оси X
void Triangle::scaleX(float f) { for (auto& v : p) { v.x *= f; } }

// Масштабирование треугольника по оси Y
void Triangle::scaleY(float f) { for (auto& v : p) { v.y *= f; } }

// Масштабирование треугольника по оси Z
void Triangle::scaleZ(float f) { for (auto& v : p) { v.z *= f; } }

// Получение нормали треугольника
Vec3d Triangle::getNormal() const {
    Vec3d ab = p[1] - p[0];
    Vec3d ac = p[2] - p[0];
    return ab.cross(ac).normalize();
}

// Проецирование треугольника (деление на w)
void Triangle::projectionDiv() {
    for (size_t i = 0; i < 3; i++) {
        // Деление координат вершины на w
        p[i].projectionDiv();
        // Деление текстурных координат на w
        t[i].projectionDiv(p[i].w);
        // Установка нового значения w
        t[i].setW(1.f / p[i].w);
    }
}

// Умножение треугольника на матрицу трансформации
Triangle Triangle::operator*(const Mat4x4& mat) {
    Triangle result = *this;
    for (size_t i = 0; i < 3; i++) {
        result.p[i] = p[i] * mat;
    }
    return result;
}

// Умножение треугольника на матрицу с присваиванием
Triangle& Triangle::operator*=(const Mat4x4& mat) {
    for (size_t i = 0; i < 3; i++) {
        p[i] = p[i] * mat;
    }
    return *this;
}

// Отрисовка текстуры на треугольник
sf::VertexArray Triangle::texturedTriangle(DepthBuffer& depthBuffer, sf::Image* texture) {
    // Извлечение координат вершин и текстурных координат
    int   y1 = p[0].y, y2 = p[1].y, y3 = p[2].y;
    int   x1 = p[0].x, x2 = p[1].x, x3 = p[2].x;
    float u1 = t[0].u, u2 = t[1].u, u3 = t[2].u;
    float v1 = t[0].v, v2 = t[1].v, v3 = t[2].v;
    float w1 = t[0].w, w2 = t[1].w, w3 = t[2].w;

    // Сортировка вершин по Y (от меньшего к большему)
    if (y2 < y1) { std::swap(y1, y2); std::swap(x1, x2); std::swap(u1, u2); std::swap(v1, v2); std::swap(w1, w2); }
    if (y3 < y1) { std::swap(y1, y3); std::swap(x1, x3); std::swap(u1, u3); std::swap(v1, v3); std::swap(w1, w3); }
    if (y3 < y2) { std::swap(y2, y3); std::swap(x2, x3); std::swap(u2, u3); std::swap(v2, v3); std::swap(w2, w3); }

    // Вычисление разностей для интерполяции
    int   dy1 = y2 - y1;
    int   dx1 = x2 - x1;
    float du1 = u2 - u1;
    float dv1 = v2 - v1;
    float dw1 = w2 - w1;

    int   dy2 = y3 - y1;
    int   dx2 = x3 - x1;
    float du2 = u3 - u1;
    float dv2 = v3 - v1;
    float dw2 = w3 - w1;

    // Текущие текстурные координаты и W
    float texU, texV, texW;

    // Шаги по X для двух сторон треугольника
    float daxStep = 0, dbxStep = 0;
    // Шаги по U и V для первой стороны
    float du1Step = 0, dv1Step = 0;
    // Шаги по U и V для второй стороны
    float du2Step = 0, dv2Step = 0;
    // Шаги по W для двух сторон
    float dw1Step = 0, dw2Step = 0;

    // Шаг по X для первой стороны
    if (dy1) daxStep = dx1 / (float)std::abs(dy1);
    // Шаг по X для второй стороны
    if (dy2) dbxStep = dx2 / (float)std::abs(dy2);

    // Шаг по U для первой стороны
    if (dy1) du1Step = du1 / (float)std::abs(dy1);
    // Шаг по V для первой стороны
    if (dy1) dv1Step = dv1 / (float)std::abs(dy1);
    // Шаг по W для первой стороны
    if (dy1) dw1Step = dw1 / (float)std::abs(dy1);

    // Шаг по U для второй стороны
    if (dy2) du2Step = du2 / (float)std::abs(dy2);
    // Шаг по V для второй стороны
    if (dy2) dv2Step = dv2 / (float)std::abs(dy2);
    // Шаг по W для второй стороны
    if (dy2) dw2Step = dw2 / (float)std::abs(dy2);

    // Массив для хранения пикселей
    sf::VertexArray pixels(sf::PrimitiveType::Points);

    // Размеры текстуры
    unsigned int texWidth, texHeight;
    // Цвет текстуры
    sf::Color texCol;
    // Цвет треугольника (если текстура не используется)
    Color triCol;

    // Если текстура включена, получаем её размеры
    if (texture && glbl::render::textureVisible) {
        texWidth = texture->getSize().x;
        texHeight = texture->getSize().y;
    }
    else {
        // Иначе используем цвет треугольника с учётом освещения
        triCol = col * illumination;
    }

    // Отрисовка верхней части треугольника (от y1 до y2)
    if (dy1) {
        for (size_t i = y1; i <= y2; i++) {
            // Вычисление начальной и конечной точек по X
            int ax = x1 + (float)(i - y1) * daxStep;
            int bx = x1 + (float)(i - y1) * dbxStep;

            // Интерполяция текстурных координат для начальной и конечной точек
            float texSu = u1 + (float)(i - y1) * du1Step;
            float texSv = v1 + (float)(i - y1) * dv1Step;
            float texSw = w1 + (float)(i - y1) * dw1Step;

            float texEu = u1 + (float)(i - y1) * du2Step;
            float texEv = v1 + (float)(i - y1) * dv2Step;
            float texEw = w1 + (float)(i - y1) * dw2Step;

            // Если начальная точка правее конечной, меняем их местами
            if (ax > bx) { std::swap(ax, bx); std::swap(texSu, texEu); std::swap(texSv, texEv); std::swap(texSw, texEw); }

            texU = texSu;
            texV = texSv;
            texW = texSw;

            // Шаг для интерполяции между начальной и конечной точками
            float tstep = 1.f / ((float)(bx - ax));
            float t = 0.f;

            // Отрисовка пикселей между начальной и конечной точками
            for (size_t j = ax; j < bx; j++) {
                // Интерполяция текстурных координат
                texU = (1.f - t) * texSu + t * texEu;
                texV = (1.f - t) * texSv + t * texEv;
                texW = (1.f - t) * texSw + t * texEw;

                // Обратное значение W для перспективной коррекции
                float wInv = 1.0f / texW;

                // Проверка буфера глубины
                if (texW > depthBuffer(i * glbl::window::width + j)) {
                    if (texture && glbl::render::textureVisible) {   
                        // Получение цвета текстуры с учётом перспективной коррекции
                        unsigned int u = static_cast<unsigned int>(std::clamp(texU * wInv * texWidth, 0.0f, static_cast<float>(texWidth-1)));
                        unsigned int v = static_cast<unsigned int>(std::clamp(texV * wInv * texHeight, 0.0f, static_cast<float>(texHeight-1)));

                        texCol = texture->getPixel({u, v});
                        // Добавление пикселя с учётом освещения
                        pixels.append(sf::Vertex{sf::Vector2f(j, i), sf::Color(texCol.r * illumination, texCol.g * illumination, texCol.b * illumination)});
                    } else {
                        // Использование цвета треугольника, если текстура не используется
                        pixels.append(sf::Vertex{sf::Vector2f(j, i), sf::Color(triCol.r, triCol.g, triCol.b)});
                    }

                    // Обновление буфера глубины
                    depthBuffer(i * glbl::window::width + j) = texW;
                }

                t += tstep;
            }
        }
    }

    // Отрисовка нижней части треугольника (от y2 до y3) по такому же алгоритму как и верхнюю часть
    dy1 = y3 - y2;
    dx1 = x3 - x2;
    du1 = u3 - u2;
    dv1 = v3 - v2;
    dw1 = w3 - w2;

    if (dy1) daxStep = dx1 / (float)std::abs(dy1);
    if (dy2) dbxStep = dx2 / (float)std::abs(dy2);

    du1Step = 0; dv1Step = 0;
    if (dy1) du1Step = du1 / (float)std::abs(dy1);
    if (dy1) dv1Step = dv1 / (float)std::abs(dy1);
    if (dy1) dw1Step = dw1 / (float)std::abs(dy1);

    if (dy1) {
        for (size_t i = y2; i <= y3; i++) {
            int ax = x2 + (float)(i - y2) * daxStep;
            int bx = x1 + (float)(i - y1) * dbxStep;

            float texSu = u2 + (float)(i - y2) * du1Step;
            float texSv = v2 + (float)(i - y2) * dv1Step;
            float texSw = w2 + (float)(i - y2) * dw1Step;

            float texEu = u1 + (float)(i - y1) * du2Step;
            float texEv = v1 + (float)(i - y1) * dv2Step;
            float texEw = w1 + (float)(i - y1) * dw2Step;

            if (ax > bx) { std::swap(ax, bx); std::swap(texSu, texEu); std::swap(texSv, texEv); std::swap(texSw, texEw); }

            texU = texSu;
            texV = texSv;
            texW = texSw;

            float tstep = 1.f / ((float)(bx - ax));
            float t = 0.f;

            for (size_t j = ax; j < bx; j++) {
                texU = (1.f - t) * texSu + t * texEu;
                texV = (1.f - t) * texSv + t * texEv;
                texW = (1.f - t) * texSw + t * texEw;
                
                float wInv = 1.0f / texW;
                
                if (texW > depthBuffer(i * glbl::window::width + j)) {
                    if (texture && glbl::render::textureVisible) {   
                        unsigned int u = static_cast<unsigned int>(std::clamp(texU * wInv * texWidth, 0.0f, static_cast<float>(texWidth-1)));
                        unsigned int v = static_cast<unsigned int>(std::clamp(texV * wInv * texHeight, 0.0f, static_cast<float>(texHeight-1)));

                        texCol = texture->getPixel({u, v});
                        pixels.append(sf::Vertex{sf::Vector2f(j, i), sf::Color(texCol.r * illumination, texCol.g * illumination, texCol.b * illumination)});
                    } else {
                        pixels.append(sf::Vertex{sf::Vector2f(j, i), sf::Color(triCol.r, triCol.g, triCol.b)});
                    }

                    depthBuffer(i * glbl::window::width + j) = texW;
                }
                
                t += tstep;
            }
        }
    }

    return pixels;
}

// Отсечение треугольника относительно плоскости
int Triangle::clipAgainsPlane(const Vec3d& planePoint, const Vec3d& planeNormal, const Triangle& inTri, Triangle& outTri1, Triangle& outTri2) {
    // Нормализация нормали плоскости
    Vec3d normalizedPlaneNormal = planeNormal.normalize();

    // Лямбда-функция для вычисления расстояния от точки до плоскости
    auto dist = [&](const Vec3d& point) {
        return normalizedPlaneNormal.dot(point) - normalizedPlaneNormal.dot(planePoint);
    };

    // Вершины внутри плоскости
    const Vec3d* insidePoints[3];
    // Вершины снаружи плоскости
    const Vec3d* outsidePoints[3];
    // Количество вершин внутри и снаружи
    int insidePointCount = 0, outsidePointCount = 0;

    // Текстурные координаты внутри плоскости
    const Vec2d* insideTex[3];
    // Текстурные координаты снаружи плоскости
    const Vec2d* outsideTex[3];
    // Количество текстурных координат внутри и снаружи
    int insideTexCount = 0, outsideTexCount = 0;

    // Вычисление расстояний от вершин треугольника до плоскости
    float d0 = dist(inTri.p[0]);
    float d1 = dist(inTri.p[1]);
    float d2 = dist(inTri.p[2]);

    // Распределение вершин и текстурных координат по массивам (внутри/снаружи плоскости)
    if (d0 >= 0) { insidePoints[insidePointCount++] = &inTri.p[0]; insideTex[insideTexCount++] = &inTri.t[0]; }
    else { outsidePoints[outsidePointCount++] = &inTri.p[0]; outsideTex[outsideTexCount++] = &inTri.t[0]; }
    if (d1 >= 0) { insidePoints[insidePointCount++] = &inTri.p[1]; insideTex[insideTexCount++] = &inTri.t[1]; }
    else { outsidePoints[outsidePointCount++] = &inTri.p[1]; outsideTex[outsideTexCount++] = &inTri.t[1]; }
    if (d2 >= 0) { insidePoints[insidePointCount++] = &inTri.p[2]; insideTex[insideTexCount++] = &inTri.t[2]; }
    else { outsidePoints[outsidePointCount++] = &inTri.p[2]; outsideTex[outsideTexCount++] = &inTri.t[2]; }

    // Если все вершины снаружи плоскости, треугольник полностью отсекается
    if (insidePointCount == 0) {
        return 0;
    }

    // Если все вершины внутри плоскости, треугольник не изменяется
    if (insidePointCount == 3) {
        outTri1 = inTri;
        return 1;
    }

    // Если одна вершина внутри плоскости, а две снаружи
    if (insidePointCount == 1 && outsidePointCount == 2) {
        // Параметр для вычисления пересечения
        float t;

        outTri1 = inTri;

        // Первая вершина нового треугольника — вершина внутри плоскости
        outTri1.p[0] = *insidePoints[0];
        outTri1.t[0] = *insideTex[0];

        // Вторая вершина — точка пересечения плоскости с ребром между вершиной внутри и первой вершиной снаружи
        outTri1.p[1].intersectPlane(planePoint, normalizedPlaneNormal, *insidePoints[0], *outsidePoints[0], t);
        outTri1.t[1].intersectPlane(*insideTex[0], *outsideTex[0], t);

        // Третья вершина — точка пересечения плоскости с ребром между вершиной внутри и второй вершиной снаружи
        outTri1.p[2].intersectPlane(planePoint, normalizedPlaneNormal, *insidePoints[0], *outsidePoints[1], t);
        outTri1.t[2].intersectPlane(*insideTex[0], *outsideTex[1], t);

        // Возвращаем один новый треугольник
        return 1;
    }

    // Если две вершины внутри плоскости, а одна снаружи
    if (insidePointCount == 2 && outsidePointCount == 1) {
        // Параметр для вычисления пересечения
        float t;

        outTri1 = inTri;

        // Первая и вторая вершины нового треугольника — вершины внутри плоскости
        outTri1.p[0] = *insidePoints[0];
        outTri1.t[0] = *insideTex[0];

        outTri1.p[1] = *insidePoints[1];
        outTri1.t[1] = *insideTex[1];

        // Третья вершина — точка пересечения плоскости с ребром между первой вершиной внутри и вершиной снаружи
        outTri1.p[2].intersectPlane(planePoint, normalizedPlaneNormal, *insidePoints[0], *outsidePoints[0], t);
        outTri1.t[2].intersectPlane(*insideTex[0], *outsideTex[0], t);

        // Второй новый треугольник
        outTri2 = inTri;

        // Первая вершина — вторая вершина внутри плоскости
        outTri2.p[0] = *insidePoints[1];
        outTri2.t[0] = *insideTex[1];

        // Вторая вершина — точка пересечения, вычисленная ранее
        outTri2.p[1] = outTri1.p[2];
        outTri2.t[1] = outTri1.t[2];

        // Третья вершина — точка пересечения плоскости с ребром между второй вершиной внутри и вершиной снаружи
        outTri2.p[2].intersectPlane(planePoint, normalizedPlaneNormal, *insidePoints[1], *outsidePoints[0], t);
        outTri2.t[2].intersectPlane(*insideTex[1], *outsideTex[0], t);

        // Возвращаем два новых треугольника
        return 2;
    }

    // Возвращаем true чтобы компилятор не жаловался, что функция может не вернуть значение, хоть это и не возможно
    return true;
}