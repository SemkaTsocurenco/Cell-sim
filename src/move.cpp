#include "../include/main.h"
#include <cmath>
#include <algorithm>

namespace {
    constexpr float kWindowWidth = 2000.0f;
    constexpr float kWindowHeight = 2000.0f;
    constexpr float kPI = 3.14159265f;
    // Если отрисовка линий взаимодействия не нужна – отключаем на этапе компиляции:
    constexpr bool kDrawInteractionLines = false;
}


void OBJ::setVelocity(float x, float y) {
    Velocity.first  = x;
    Velocity.second = y;
}

void OBJ::setAcceleration(float x, float y) {
    Acceleration.first  = x;
    Acceleration.second = y;
}

std::pair<float, float>& OBJ::getVelocity() {
    return Velocity;
}

std::pair<float, float>& OBJ::getAcceleration() {
    return Acceleration;
}

void OBJ::IncreaseVelocity(float x, float y) {
    Velocity.first  += x;
    Velocity.second += y;
}

void OBJ::IncreaseAcceleration(float x, float y) {
    Acceleration.first  += x;
    Acceleration.second += y;
}

// Обновление скорости с учётом отражения от границ окна
void OBJ::updateVelocity(float dt) {
    const float posX = Possition.first;
    const float posY = Possition.second;
    const float size = getMainSize();

    if (posX > kWindowWidth - size || posX < size)
        Velocity.first = -Velocity.first;
    if (posY > kWindowHeight - size || posY < size)
        Velocity.second = -Velocity.second;

    IncreaseVelocity(Acceleration.first * dt, Acceleration.second * dt);
}

void OBJ::updatePosition(float dt) {
    IncreasePossition(Velocity.first * dt, Velocity.second * dt);
}

// Если в будущем потребуется обновление ускорения отдельно – можно добавить логику,
// пока делегируем обновление скорости.
void OBJ::updateAcceleration(float dt) {
    updateVelocity(dt);
}



// Отрисовка линий и текста с расстоянием между объектами.
// Если kDrawInteractionLines == false, функция сразу возвращает управление.
void movement::draw_relatives(const std::pair<float, float>& posI,
                    const std::pair<float, float>& posJ,
                    float distance,
                    sf::RenderWindow& window) {
    if (!kDrawInteractionLines)
        return;

    static sf::Font font;
    static bool fontLoaded = false;
    if (!fontLoaded) {
        if (!font.loadFromFile("../KanitCyrillic.ttf")) {
            // Можно добавить обработку ошибки, если шрифт не загрузился
        }
        fontLoaded = true;
    }

    // Создаем линию между объектами
    sf::VertexArray lines(sf::Lines, 2);
    lines[0].position = sf::Vector2f(posI.first, posI.second);
    lines[1].position = sf::Vector2f(posJ.first, posJ.second);

    // Вычисляем цвет линии – пример, который можно подстроить под свои нужды
    int red = static_cast<int>(200 * (distance - 5) / 300);
    red = 255 - std::max(0, std::min(255, red));
    sf::Color lineColor(red, 0, 0);
    lines[0].color = lineColor;
    lines[1].color = lineColor;

    // Вычисляем среднюю точку и угол линии
    const float midX = (posI.first + posJ.first) * 0.5f;
    const float midY = (posI.second + posJ.second) * 0.5f;
    const float dx = posJ.first - posI.first;
    const float dy = posJ.second - posI.second;
    const float angle = std::atan2(dy, dx);

    // Отрисовка перпендикулярных линий от средней точки
    constexpr float d = 30.0f;
    const float offsetX = d * std::cos(angle + kPI / 2);
    const float offsetY = d * std::sin(angle + kPI / 2);

    sf::VertexArray Plines(sf::Lines, 4);
    Plines[0].position = sf::Vector2f(midX, midY);
    Plines[1].position = sf::Vector2f(midX + offsetX, midY + offsetY);
    Plines[2].position = sf::Vector2f(midX, midY);
    Plines[3].position = sf::Vector2f(midX - offsetX, midY - offsetY);
    for (int i = 0; i < 4; ++i)
        Plines[i].color = sf::Color::Blue;

    window.draw(Plines);

    if (dx != 0.0f || dy != 0.0f) {
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(static_cast<int>(distance)));
        text.setCharacterSize(12);
        text.setFillColor(lineColor);
        text.setPosition(midX, midY);
        text.setRotation(angle * 180 / kPI);
        window.draw(text);
    }

    window.draw(lines);
}

// Вычисление гравитационного взаимодействия между объектами.
// Перебираются только пары (i, j) с i < j, что уменьшает число итераций в 2 раза.
// Вместо деления на (distance*distance) и последующего деления на distance – используется обратная кубическая зависимость.
void movement::relate(std::vector<OBJ*>& objects, float G, float dt, sf::RenderWindow& window) {
    const size_t n = objects.size();
    for (size_t i = 0; i < n; ++i) {
        const float sizeI = objects[i]->getMainSize();
        // Центр объекта i
        const float posIX = objects[i]->getPossition().first ;
        const float posIY = objects[i]->getPossition().second ;
        for (size_t j = i + 1; j < n; ++j) {
            const float sizeJ = objects[j]->getMainSize();
            const float posJX = objects[j]->getPossition().first ;
            const float posJY = objects[j]->getPossition().second ;
            
            const float dx = posJX - posIX;
            const float dy = posJY - posIY;
            float distSq = dx * dx + dy * dy;

            const float minDist = sizeI + sizeJ;
            const float minDistSq = minDist * minDist;
            if (distSq < minDistSq)
                distSq = minDistSq;
            const float distance = std::sqrt(distSq);
            if (distance > 50 ){
            const float invDist = 1.0f / distance;
            const float invDist3 = invDist * invDist * invDist;

            // Гравитационная сила: F = G * m / (distance^2), а ускорение – F/distance,
            // что эквивалентно G * m * dx / (distance^3)
            const float force = G * objects[j]->getMass() * invDist3;
            const float ax = force * dx;
            const float ay = force * dy;

            objects[i]->IncreaseAcceleration(ax, ay);
            objects[j]->IncreaseAcceleration(-ax, -ay);
            }
            draw_relatives({ posIX, posIY }, { posJX, posJY }, distance, window);
        }
    }
}

// Обработка столкновений с использованием сравнения квадратов расстояний для избежания вызова std::sqrt.
void movement::handleCollisions(std::vector<OBJ*>& objects, float restitution, sf::RenderWindow& window) {
    const size_t n = objects.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            const float sizeI = objects[i]->getMainSize();
            const float sizeJ = objects[j]->getMainSize();
            const auto& posI = objects[i]->getPossition();
            const auto& posJ = objects[j]->getPossition();
            const float dx = (posJ.first + sizeJ) - (posI.first + sizeI);
            const float dy = (posJ.second + sizeJ) - (posI.second + sizeI);
            const float distSq = dx * dx + dy * dy;
            const float minDist = sizeI + sizeJ;
            if (distSq < (minDist * minDist)) {
                const auto velI = objects[i]->getVelocity();
                const auto velJ = objects[j]->getVelocity();
                objects[i]->setVelocity(-velI.first * restitution, -velI.second * restitution);
                objects[j]->setVelocity(-velJ.first * restitution, -velJ.second * restitution);
            }
        }
    }
}

// Обновление объектов: сначала скорость (с учётом ускорения), затем положение.
void movement::update(std::vector<OBJ*>& objects, float dt) {
    for (auto& obj : objects) {
        obj->updateVelocity(dt);
        obj->updatePosition(dt);
    }
}


