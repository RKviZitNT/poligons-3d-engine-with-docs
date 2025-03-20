#include "Engine.hpp"

// Конструктор
Engine::Engine() :
    // Вызываем конструктор создания окна
    m_window(sf::VideoMode({glbl::window::width, glbl::window::height}), "3d render", sf::Style::Titlebar | sf::Style::Close),
    // Пауза по умолчанию выключена
    m_isPaused(false),
    // Курсор мыши заблокирован по умолчанию
    m_isMouseLocked(true),
    // Инициализация рендера с камерой
    m_render(m_camera),
    // Загрузка модели и текстуры
    m_cube("resources/models/level.obj", "resources/textures/leveltexhigh.png")
{
    // Скрытие курсора мыши, если он заблокирован
    m_window.setMouseCursorVisible(!m_isMouseLocked);
    // Вычисление центра окна
    windowCenter = sf::Vector2i(m_window.getSize().x / 2, m_window.getSize().y / 2);

    // Скорость перемещения камеры
    m_cameraTranslateSpeed = 2;
    // Скорость вращения камеры
    m_cameraRotateSpeed = 0.001;

    // Перемещение модели
    m_cube.translate({0, 0, 2});
    // Масштабирование модели
    m_cube.scale({0.2, 0.2, 0.2});
    // Добавление модели в рендерер
    m_render.addMesh(m_cube);
    // Установка направления света
    m_light.setDir({0.8, 1, -0.5});
}

// Основной цикл приложения
void Engine::run() {
    // Таймер для измерения времени
    sf::Clock clock;
    // Время одного кадра
    sf::Time frameTime = sf::seconds(1.f / glbl::window::frameRate);

    // Время с последнего обновления FPS
    sf::Time elapsedTimeSinceLastUpdate = sf::Time::Zero;

    // Установка курсора мыши в центр окна
    sf::Mouse::setPosition(windowCenter, m_window);

    // Основной цикл
    while (m_window.isOpen()) {
        // Время, прошедшее с последнего кадра
        deltaTime = clock.restart();
        elapsedTimeSinceLastUpdate += deltaTime;

        // Обработка событий
        handleEvents();

        // Пропуск обновления и отрисовки, если приложение на паузе
        if (m_isPaused) { continue; }

        // Обновление состояния
        update();
        // Отрисовка сцены
        draw();

        // Ограничение FPS
        sf::Time elapsedTime = clock.getElapsedTime();
        if (elapsedTime < frameTime) {
            sf::sleep(frameTime - elapsedTime);
        }

        // Обновление заголовка окна (FPS)
        if (elapsedTimeSinceLastUpdate >= sf::seconds(0.05f)) {
            float fps = 1.f / deltaTime.asSeconds(); 
            m_window.setTitle("3d render - FPS: " + std::to_string(static_cast<int>(fps)));
            elapsedTimeSinceLastUpdate = sf::Time::Zero;
        }
    }
}

// Обработка событий
void Engine::handleEvents() {
    // Обработка всех событий в очереди
    while (const std::optional event = m_window.pollEvent()) {
        // Закрытие окна
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }

        // Потеря фокуса (пауза)
        if (event->is<sf::Event::FocusLost>()) {
            m_isPaused = true;
        }

        // Возврат фокуса (снятие паузы)
        if (event->is<sf::Event::FocusGained>()) {
            m_isPaused = false;
        }

        // Нажатие клавиши
        if (event->is<sf::Event::KeyPressed>()) {
            auto eventKeyPressed = event->getIf<sf::Event::KeyPressed>();
            // Переключение блокировки мыши
            if (eventKeyPressed->code == sf::Keyboard::Key::Escape) {
                m_isMouseLocked = !m_isMouseLocked;
                sf::Mouse::setPosition(windowCenter, m_window);
                m_window.setMouseCursorVisible(!m_isMouseLocked);
            }
        }
    }

    // Управление камерой с клавиатуры
    float translateSpeed = m_cameraTranslateSpeed * deltaTime.asSeconds();

    // Вперёд
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { m_camera.translateForwardNoY(translateSpeed); }
    // Влево
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) { m_camera.translateLeft(translateSpeed); }
    // Назад
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) { m_camera.translateBackNoY(translateSpeed); }
     // Вправо
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) { m_camera.translateRight(translateSpeed); }
    // Вверх
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) { m_camera.translateUp(translateSpeed); }
    // Вниз
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) { m_camera.translateDown(translateSpeed); }

    // Управление камерой мышью
    if (m_isMouseLocked) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
        sf::Vector2i delta = mousePosition - windowCenter;

        // Вращение по горизонтали
        if (delta.x != 0) { m_camera.rotateHorizontal(delta.x * m_cameraRotateSpeed); }
        // Вращение по вертикали
        if (delta.y != 0) { m_camera.rotateVertical(-delta.y * m_cameraRotateSpeed); }

        // Возврат курсора в центр окна
        sf::Mouse::setPosition(windowCenter, m_window);
    }
}

// Обновление состояния
void Engine::update() {
    // Обновление рендера
    m_render.update();
}

// Отрисовка сцены
void Engine::draw() {
    // Очистка экрана
    m_window.clear(sf::Color::Black);

    // Отрисовка сцены
    m_render.render(m_window, m_light);

    // Отображение кадра
    m_window.display();
}
