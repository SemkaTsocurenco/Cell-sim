#include "../include/main.h"


Branch::Branch(int lvl, sf::RenderWindow& win, const Leaf& leaf)
    : level(lvl), window(win), myLeaf(leaf)
{
    if (level < MaxDepth) {
        FillOut();
    }
}

void Branch::FillOut() {
    // Копируем границы текущего листа
    float minX = myLeaf.minX;
    float minY = myLeaf.minY;
    float maxX = myLeaf.maxX;
    float maxY = myLeaf.maxY;
    
    // Вычисляем середину для деления на 4 квадранта
    float midX = (minX + maxX) / 2.0f;
    float midY = (minY + maxY) / 2.0f;

    // Создаём 4 новых Leaf для каждого квадранта; вектор объектов изначально пустой
    Leaf NW(minX, minY, midX, midY, NWObj);
    Leaf NE(midX, minY, maxX, midY, NEObj);
    Leaf SW(minX, midY, midX, maxY, SWObj);
    Leaf SE(midX, midY, maxX, maxY, SEObj);

    // Распределяем объекты из текущего Leaf по новым квадрантам
    for (auto obj : myLeaf.objects) {
        auto pos = obj->getPossition();
        if (pos.first >= NW.minX && pos.first < NW.maxX &&
            pos.second >= NW.minY && pos.second < NW.maxY)
        {
            NW.objects.push_back(obj);
        }
        else if (pos.first >= NE.minX && pos.first < NE.maxX &&
                 pos.second >= NE.minY && pos.second < NE.maxY)
        {
            NE.objects.push_back(obj);
        }
        else if (pos.first >= SW.minX && pos.first < SW.maxX &&
                 pos.second >= SW.minY && pos.second < SW.maxY)
        {
            SW.objects.push_back(obj);
        }
        else if (pos.first >= SE.minX && pos.first < SE.maxX &&
                 pos.second >= SE.minY && pos.second < SE.maxY)
        {
            SE.objects.push_back(obj);
        }
    }

    // Отрисовываем границы каждого квадранта
    DrawLeaf(NW);
    DrawLeaf(NE);
    DrawLeaf(SW);
    DrawLeaf(SE);

    // Рекурсивно строим новые ветви, если в квадранте больше одного объекта
    if (NW.objects.size() > 1)
        Branch(level + 1, window, NW);
    if (NE.objects.size() > 1)
        Branch(level + 1, window, NE);
    if (SW.objects.size() > 1)
        Branch(level + 1, window, SW);
    if (SE.objects.size() > 1)
        Branch(level + 1, window, SE);
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
