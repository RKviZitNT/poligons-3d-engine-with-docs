#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Config.hpp"
#include "math/Mat4x4.hpp"
#include "math/Vec3d.hpp"
#include "math/Vec2d.hpp"
#include "components/geometry/Triangle.hpp"
#include "components/Camera.hpp"
#include "components/props/Color.hpp"

// Класс для работы с 3D-моделями
class Mesh {
public:
    // Конструктор c загрузкой модели из файла
    Mesh(const std::string& modelFilename);
    // Конструктор c загрузкой модели и текстуры из файла
    Mesh(const std::string& modelFilename, const std::string& textureFilename);

    // Инициализация параметров модели (позиция, масштаб, угол)
    void init();
    // Проверка, есть ли текстура у модели
    bool isTextured();
    // Получение текстуры модели
    sf::Image* getTexture();

    // Перемещение модели
    void translate(const Vec3d& offset);
    // Масштабирование модели
    void scale(const Vec3d& scale);
    // Вращение модели
    void rotate(const Vec3d& angle);

    // Получение трансформированных треугольников модели
    std::vector<Triangle> getTransformedTriangles() const;

private:
    // Треугольники модели
    std::vector<Triangle> m_triangles;
    // Вершины модели
    std::vector<Vec3d> m_vertices;
    // Текстурные координаты
    std::vector<Vec2d> m_textureCoords;

    // Позиция, масштаб и углы вращения модели
    Vec3d m_position, m_scale, m_angle;

    // Текстура модели
    sf::Image* m_texture = nullptr;

    // Загрузка модели из файла .obj
    void loadModel(std::string filename);
    // Загрузка текстуры
    void loadTexture(std::string filename);

    // Обработка строки файла .obj
    void parseLine(std::string& line);

    // Извлечение индекса вершины
    int extractVertexIndex(const std::string& token);
    // Извлечение индекса текстуры
    int extractTextureIndex(const std::string& token);
};