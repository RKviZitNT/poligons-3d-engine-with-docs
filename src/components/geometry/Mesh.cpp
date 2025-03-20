#include "components/geometry/Mesh.hpp"

// Конструктор для загрузки модели без текстуры
Mesh::Mesh(const std::string& modelFilename) {
    // Загрузка модели
    loadModel(modelFilename);
    // Инициализация параметров
    init();
}

// Конструктор для загрузки модели с текстурой
Mesh::Mesh(const std::string& modelFilename, const std::string& textureFilename) {
    // Загрузка модели
    loadModel(modelFilename);
    // Загрузка текстуры
    loadTexture(textureFilename);
    // Инициализация параметров
    init();
}

// Инициализация параметров модели
void Mesh::init() {
    // Позиция по умолчанию (0, 0, 0)
    m_position = Vec3d(0);
    // Масштаб по умолчанию (1, 1, 1)
    m_scale = Vec3d(1);
    // Углы вращения по умолчанию (0, 0, 0)
    m_angle = Vec3d(0);
}

// Получение текстуры модели
sf::Image* Mesh::getTexture() { return m_texture; }

// Загрузка модели из файла .obj
void Mesh::loadModel(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Ошибка, если файл не открылся
        throw std::runtime_error("Failed to open obj file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Парсинг каждой строки файла
        parseLine(line);
    }

    file.close();
}

// Загрузка текстуры
void Mesh::loadTexture(std::string filename) {
    m_texture = new sf::Image;
    if (!m_texture->loadFromFile(filename)) {
        // Ошибка, если текстура не загрузилась
        throw std::runtime_error("Failed to load texture");
    }
}

// Парсинг строки файла .obj
void Mesh::parseLine(std::string& line) {
    std::istringstream iss(line);
    std::string prefix;
    // Чтение префикса строки (v, vt, f и т.д.)
    iss >> prefix;

    // Вершина
    if (prefix == "v") {
        Vec3d vertex;
        // Чтение координат вершины
        iss >> vertex.x >> vertex.y >> vertex.z;
        // Добавление вершины в список
        m_vertices.push_back(vertex);
    }

    // Текстурная координата
    else if (prefix == "vt") {
        Vec2d texCoord;
        // Чтение текстурных координат
        iss >> texCoord.u >> texCoord.v;
        // Инверсия координаты V (для корректного отображения, такова специфика движка)
        texCoord.v = 1.0f - texCoord.v;
        // Добавление текстурной координаты в список
        m_textureCoords.push_back(texCoord);
    }

    // Треугольник
    else if (prefix == "f") {
        // Индексы вершин
        std::vector<int> vertexIndices;
        // Индексы текстурных координат
        std::vector<int> textureIndices;
        std::string token;

        // Чтение токенов (вершина/текстура)
        while (iss >> token) {
            // Извлечение индекса вершины
            int vIdx = extractVertexIndex(token);
            // Извлечение индекса текстуры
            int tIdx = extractTextureIndex(token);
            // Добавление индекса вершины
            vertexIndices.push_back(vIdx);
            // Добавление индекса текстуры
            textureIndices.push_back(tIdx);
        }

        // Формирование треугольников из вершин
        for (size_t i = 1; i + 1 < vertexIndices.size(); ++i) {
            Triangle tri(
                m_vertices[vertexIndices[0]],
                m_vertices[vertexIndices[i]],
                m_vertices[vertexIndices[i + 1]]
            );

            // Установка текстурных координат, если они есть
            if (!textureIndices.empty() && textureIndices[0] != -1) {
                tri.setTextureCoords(
                    m_textureCoords[textureIndices[0]],
                    m_textureCoords[textureIndices[i]],
                    m_textureCoords[textureIndices[i + 1]]
                );
            }

            // Добавление треугольника в список
            m_triangles.push_back(tri);
        }
    }
}

// Извлечение индекса вершины из токена
int Mesh::extractVertexIndex(const std::string& token) {
    size_t pos = token.find('/');
    return (pos != std::string::npos) ? std::stoi(token.substr(0, pos)) - 1 : std::stoi(token) - 1;
}

// Извлечение индекса текстуры из токена
int Mesh::extractTextureIndex(const std::string& token) {
    size_t firstSlash = token.find('/');
    // Если нет текстуры, возвращаем -1
    if (firstSlash == std::string::npos) return -1;

    size_t secondSlash = token.find('/', firstSlash + 1);
    if (secondSlash == std::string::npos) {
        // Извлечение индекса текстуры
        return std::stoi(token.substr(firstSlash + 1)) - 1;
    }

    return std::stoi(token.substr(firstSlash + 1, secondSlash - firstSlash - 1)) - 1;
}

// Перемещение модели
void Mesh::translate(const Vec3d& offset) {
    m_position += offset;
}

// Масштабирование модели
void Mesh::scale(const Vec3d& scale) {
    m_scale *= scale;
}

// Вращение модели
void Mesh::rotate(const Vec3d& angle) {
    m_angle += angle;
}

// Получение трансформированных треугольников модели
std::vector<Triangle> Mesh::getTransformedTriangles() const {
    Mat4x4 matTrans, matScl, matRot;

    // Матрица перемещения
    matTrans = Mat4x4::translation(m_position.x, m_position.y, m_position.z);
    // Матрица масштабирования
    matScl = Mat4x4::scale(m_scale.x, m_scale.y, m_scale.z);
    // Матрица вращения
    matRot = Mat4x4::rotationX(m_angle.x) * Mat4x4::rotationY(m_angle.y) * Mat4x4::rotationZ(m_angle.z);

    std::vector<Triangle> transformedTriangles;
    for (auto triangle : m_triangles) {
        // Применение масштабирования
        triangle *= matScl;
        // Применение вращения
        triangle *= matRot;
        // Применение перемещения
        triangle *= matTrans;
        // Добавление трансформированного треугольника в конечный вектор
        transformedTriangles.emplace_back(triangle);
    }

    return transformedTriangles;
}