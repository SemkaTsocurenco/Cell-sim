#include "../include/main.h"


Branch::Branch(int lvl, sf::RenderWindow& win, const Leaf& leaf)
    : level(lvl), window(win), myLeaf(leaf), totalMass(0.f), centerX(0.f), centerY(0.f)
{
    // Если достигнута максимальная глубина или объектов мало – не делим дальше.
    if (level < MaxDepth && myLeaf.objects.size() > Threshold) {
        FillOut();
    }
    // Если узел не делится, считаем его листом.
    else {
        // Суммируем массу всех объектов и вычисляем центр масс.
        float sumMass = 0.f;
        float sumX = 0.f;
        float sumY = 0.f;
        for (auto obj : myLeaf.objects) {
            float m = obj->getMass();
            sumMass += m;
            sumX += m * obj->getPossition().first;
            sumY += m * obj->getPossition().second;
        }
        totalMass = sumMass;
        if (sumMass > 0.f) {
            centerX = sumX / sumMass;
            centerY = sumY / sumMass;
        } else {
            // Если нет объектов – берем центр области
            centerX = (myLeaf.minX + myLeaf.maxX) / 2.0f;
            centerY = (myLeaf.minY + myLeaf.maxY) / 2.0f;
        }
    }
}

void Branch::FillOut() {
    // Копируем границы текущего листа.
    float minX = myLeaf.minX;
    float minY = myLeaf.minY;
    float maxX = myLeaf.maxX;
    float maxY = myLeaf.maxY;
    
    // Вычисляем середину для деления на 4 квадранта.
    float midX = (minX + maxX) / 2.0f;
    float midY = (minY + maxY) / 2.0f;

    // Распределяем объекты по 4 новым векторaм.
    std::vector<OBJ*> NWObjs;
    std::vector<OBJ*> NEObjs;
    std::vector<OBJ*> SWObjs;
    std::vector<OBJ*> SEObjs;

    for (auto obj : myLeaf.objects) {
        auto pos = obj->getPossition();
        if (pos.first >= minX && pos.first < midX &&
            pos.second >= minY && pos.second < midY)
        {
            NWObjs.push_back(obj);
        }
        else if (pos.first >= midX && pos.first < maxX &&
                 pos.second >= minY && pos.second < midY)
        {
            NEObjs.push_back(obj);
        }
        else if (pos.first >= minX && pos.first < midX &&
                 pos.second >= midY && pos.second < maxY)
        {
            SWObjs.push_back(obj);
        }
        else if (pos.first >= midX && pos.first < maxX &&
                 pos.second >= midY && pos.second < maxY)
        {
            SEObjs.push_back(obj);
        }
    }

    // Создаём 4 новых Leaf для каждого квадранта.
    Leaf NW(minX, minY, midX, midY, NWObjs);
    Leaf NE(midX, minY, maxX, midY, NEObjs);
    Leaf SW(minX, midY, midX, maxY, SWObjs);
    Leaf SE(midX, midY, maxX, maxY, SEObjs);

    // Отрисовываем границы каждого квадранта (для отладки).
    // DrawLeaf(NW);
    // DrawLeaf(NE);
    // DrawLeaf(SW);
    // DrawLeaf(SE);

    // Рекурсивно создаём дочерние ветви, если количество объектов больше порога.
    if (NW.objects.size() > Threshold)
        NW_node = new Branch(level + 1, window, NW);
    if (NE.objects.size() > Threshold)
        NE_node = new Branch(level + 1, window, NE);
    if (SW.objects.size() > Threshold)
        SW_node = new Branch(level + 1, window, SW);
    if (SE.objects.size() > Threshold)
        SE_node = new Branch(level + 1, window, SE);

    // Теперь вычисляем суммарную массу и центр масс для текущего узла.
    float massNW = 0.f, massNE = 0.f, massSW = 0.f, massSE = 0.f;
    float centerNW_x = 0.f, centerNW_y = 0.f;
    float centerNE_x = 0.f, centerNE_y = 0.f;
    float centerSW_x = 0.f, centerSW_y = 0.f;
    float centerSE_x = 0.f, centerSE_y = 0.f;


    static sf::Font font;
    static bool fontLoaded = false;
    if (!fontLoaded) {
        if (!font.loadFromFile("../res/KanitCyrillic.ttf")) {
            // Можно добавить обработку ошибки, если шрифт не загрузился
        }
        fontLoaded = true;
    }

    // sf::Text totalMass_text;
    // totalMass_text.setCharacterSize(10.0f);
    // totalMass_text.setFillColor(sf::Color::Magenta);
    // totalMass_text.setFont(font);
    // sf::CircleShape centerMass_draw;
    // centerMass_draw.setFillColor(sf::Color::Blue);
    // centerMass_draw.setRadius(5.0f);



    // Для каждого квадранта: если дочерняя ветвь создана – используем её данные,
    // иначе суммируем объекты из соответствующего Leaf.
    if (NW_node) {
        massNW = NW_node->totalMass;
        centerNW_x = NW_node->centerX;
        centerNW_y = NW_node->centerY;
    } else {
        for (auto obj : NW.objects) {
            float m = obj->getMass();
            massNW += m;
            centerNW_x += m * obj->getPossition().first;
            centerNW_y += m * obj->getPossition().second;
        }
        if (massNW > 0.f) {
            centerNW_x /= massNW;
            centerNW_y /= massNW;
        }

        // totalMass_text.setPosition(minX, minY);
        // totalMass_text.setString(std::to_string(static_cast<int>(massNW)));
        // window.draw(totalMass_text);
        // centerMass_draw.setPosition( centerNW_x - 5.0f ,centerNW_y - 5.0f );
        // window.draw(centerMass_draw);
    }
    if (NE_node) {
        massNE = NE_node->totalMass;
        centerNE_x = NE_node->centerX;
        centerNE_y = NE_node->centerY;
    } else {
        for (auto obj : NE.objects) {
            float m = obj->getMass();
            massNE += m;
            centerNE_x += m * obj->getPossition().first;
            centerNE_y += m * obj->getPossition().second;
        }
        if (massNE > 0.f) {
            centerNE_x /= massNE;
            centerNE_y /= massNE;
        }

        // totalMass_text.setPosition(midX, minY);
        // totalMass_text.setString(std::to_string(static_cast<int>(massNE)));
        // window.draw(totalMass_text);
        // centerMass_draw.setPosition( centerNE_x - 5.0f ,centerNE_y - 5.0f );
        // window.draw(centerMass_draw);
    }
    if (SW_node) {
        massSW = SW_node->totalMass;
        centerSW_x = SW_node->centerX;
        centerSW_y = SW_node->centerY;
    } else {
        for (auto obj : SW.objects) {
            float m = obj->getMass();
            massSW += m;
            centerSW_x += m * obj->getPossition().first;
            centerSW_y += m * obj->getPossition().second;
        }
        if (massSW > 0.f) {
            centerSW_x /= massSW;
            centerSW_y /= massSW;
        }
            // totalMass_text.setPosition(minX, midY);
            // totalMass_text.setString(std::to_string(static_cast<int>(massSW)));
            // window.draw(totalMass_text);
            // centerMass_draw.setPosition( centerSW_x - 5.0f ,centerSW_y - 5.0f );
            // window.draw(centerMass_draw);
    }
    if (SE_node) {
        massSE = SE_node->totalMass;
        centerSE_x = SE_node->centerX;
        centerSE_y = SE_node->centerY;
    } else {
        for (auto obj : SE.objects) {
            float m = obj->getMass();
            massSE += m;
            centerSE_x += m * obj->getPossition().first;
            centerSE_y += m * obj->getPossition().second;
        }
        if (massSE > 0.f) {
            centerSE_x /= massSE;
            centerSE_y /= massSE;
        }

        // totalMass_text.setPosition(midX, midY);
        // totalMass_text.setString(std::to_string(static_cast<int>(massSE)));
        // window.draw(totalMass_text);
        // centerMass_draw.setPosition( centerSE_x - 5.0f ,centerSE_y - 5.0f );
        // window.draw(centerMass_draw);
    }

    totalMass = massNW + massNE + massSW + massSE;
    if (totalMass > 0.f) {
        centerX = (centerNW_x * massNW + centerNE_x * massNE +
                   centerSW_x * massSW + centerSE_x * massSE) / totalMass;
        centerY = (centerNW_y * massNW + centerNE_y * massNE +
                   centerSW_y * massSW + centerSE_y * massSE) / totalMass;
    } else {
        centerX = (minX + maxX) / 2.0f;
        centerY = (minY + maxY) / 2.0f;
    }








}

void Branch::DrawLeaf(const Leaf& leaf) {
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(0.5f);
    rect.setSize({ leaf.maxX - leaf.minX, leaf.maxY - leaf.minY });
    rect.setPosition(leaf.minX, leaf.minY);
    window.draw(rect);
}
