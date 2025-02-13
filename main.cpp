#include "./include/main.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(2000, 2000), "Gravitational Interaction", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60); // Повышенная частота кадров может обеспечить плавность

    // Инициализация генератора случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posXDist(50, 1950);
    std::uniform_real_distribution<float> posYDist(50, 1950);
    std::uniform_real_distribution<float> massDist(300.0f, 1500.0f);
    std::uniform_real_distribution<float> radiusDist(1.0f, 1.0f);

    // Создание объектов (например, кругов)
    std::vector<circle> circles;
    float minX = 90000, maxX = -100, minY = 90000, maxY = -100;
    for (int i = 0; i < 3000; i++) {
         float x = posXDist(gen);
         float y = posYDist(gen);
         float mass = massDist(gen);
         float radius = radiusDist(gen);



         int red = static_cast<int>(255 * (mass - massDist.a()) / (massDist.b() - massDist.a()));
         int green = static_cast<int>(255 * (massDist.b() - mass) / (massDist.b() - massDist.a()));
         sf::Color color(red, 0, 0);
         circles.emplace_back(std::make_pair(x-radius, y-radius), radius, color, mass, 0.8f , sf::Color::Red);
    }


     std::vector<OBJ*> objects;



    // Вектор указателей на объекты базового класса
    for (auto& c : circles) {
         objects.push_back(&c);
    }

    sf::Clock clock;
    while (window.isOpen())
    {
         float dt = clock.restart().asSeconds();

         sf::Event event;
         while (window.pollEvent(event))
         {
              if (event.type == sf::Event::Closed)
                  window.close();
         }

         window.clear(sf::Color::White);



          // window.draw(BBox);

         // Сброс ускорения у каждого объекта
         for (auto& obj : objects) {
             obj->setAcceleration(0, 0);
         }

               float minX = 99999;
               float minY = 99999;
               float maxX= -99999;
               float maxY= -99999;

     //     Вычисление гравитационного взаимодействия, обработка столкновений и обновление объектов
         movement::relate(objects, 10.0f, dt, window);
     //     movement::handleCollisions(objects, 0.5f, window);
          for (auto& obj : objects) {
               obj->updateVelocity(dt);
               obj->updatePosition(dt);
               float x = obj->getPossition().first;
               float y = obj->getPossition().second;

               minX = std::min(minX, x);
               minY = std::min(minY, y);
               maxX = std::max(maxX, x);
               maxY = std::max(maxY, y);
          }


         Leaf mainLeaf {minX, minY, maxX, maxY, objects};

         Branch(1, window, mainLeaf);

         // Отрисовка объектов
         for (auto& obj : objects) {
              obj->draw(window);
         }

         window.display();
         
    }

    return 0;
}
