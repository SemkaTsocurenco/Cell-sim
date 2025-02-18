#include "../include/main.h"
#include <cmath>
#include <algorithm>


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

    // if (posX > kWindowWidth - size || posX < size)
    //     Velocity.first = -Velocity.first;
    // if (posY > kWindowHeight - size || posY < size)
    //     Velocity.second = -Velocity.second;

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


//-------------------------------------------------------------------------
//    Для центра масс, если узел далеко.
//-------------------------------------------------------------------------

void movement::addReactionFromNodeAsSingleMass(Branch& node, OBJ& obj)
{
    float objX = obj.getPossition().first;
    float objY = obj.getPossition().second;
    float dx = node.centerX - objX;
    float dy = node.centerY - objY;
    float distSq = dx*dx + dy*dy;

    float minDist = 2.0f * obj.getMainSize();  
    float minDistSq = minDist * minDist;
    if (distSq < minDistSq) {
        distSq = minDistSq;
    }
    float dist = std::sqrt(distSq);
    float invDist = 1.0f / dist;
    float invDist3 = invDist * invDist * invDist;

    float force = G * node.totalMass * invDist3;

    float ax = force * dx;
    float ay = force * dy;

    obj.IncreaseAcceleration(ax, ay);
}

//-------------------------------------------------------------------------
//    Если узел – лист, где хранится несколько объектов,
//    можем сделать прямой перебор «объект-объект» (парное взаимодействие).
//    Здесь допустим применять третий закон (добавлять +ax / -ax).
//-------------------------------------------------------------------------
void movement::addReactionLeafPairwise(Branch& node, OBJ& obj)
{
    auto& objectsInNode = node.myLeaf.objects;
    for (auto* other : objectsInNode) {
        if (other == &obj) {
            continue; // не взаимодействуем объект сам с собой
        }

        float dx = other->getPossition().first  - obj.getPossition().first;
        float dy = other->getPossition().second - obj.getPossition().second;
        float distSq = dx*dx + dy*dy;

        // Минимальная дистанция — сумма радиусов (или размеров)
        float minDist = other->getMainSize() + obj.getMainSize();
        float minDistSq = minDist * minDist;
        if (distSq < minDistSq) {
            distSq = minDistSq;
        }

        float dist = std::sqrt(distSq);
        float invDist = 1.0f / dist;
        float invDist3 = invDist * invDist * invDist;

        float force = G * other->getMass() * invDist3;
        float ax = force * dx;
        float ay = force * dy;

        // Применяем силу и "третьим законом" обратно
        obj.IncreaseAcceleration(ax, ay);
        other->IncreaseAcceleration(-ax, -ay);
    }
}


void movement::calculateReaction(Branch& node, OBJ& obj)
{
    if (node.myLeaf.objects.empty()) {
        return;
    }

    bool isLeaf = (!node.NW_node && !node.NE_node && !node.SW_node && !node.SE_node);
    if (isLeaf && node.myLeaf.objects.size() > 1) {
        addReactionLeafPairwise(node, obj);
        return;
    }

    float sizeQuadrant = node.myLeaf.maxX - node.myLeaf.minX;
    float dx = obj.getPossition().first  - node.centerX;
    float dy = obj.getPossition().second - node.centerY;
    float distToObj = std::sqrt(dx*dx + dy*dy);
    if (distToObj > 50.0f){

    // Если узел достаточно "далёк" => приближаем всю массу узла одной точкой
    if ((sizeQuadrant / distToObj) < theta) {
        addReactionFromNodeAsSingleMass(node, obj);

    } else {
        // Узел слишком близко => спускаемся к детям (если они есть)
        if (node.NW_node) calculateReaction(*node.NW_node, obj);
        if (node.NE_node) calculateReaction(*node.NE_node, obj);
        if (node.SW_node) calculateReaction(*node.SW_node, obj);
        if (node.SE_node) calculateReaction(*node.SE_node, obj);
    }
    }
}


void movement::relate(std::vector<OBJ*>& objects, Branch& root)
{
    // Для каждого объекта обнуляем ускорение и обходим дерево
    for (auto* obj : objects) {
        obj->setAcceleration(0.f, 0.f); 
        calculateReaction(root, *obj);
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


