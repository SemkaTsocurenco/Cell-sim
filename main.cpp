#include "./include/main.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(kWindowWidth, kWindowHeight), "Gravitational Interaction", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(15); 

    // Инициализация генератора случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    float centerX = kWindowWidth * 0.5f;
    float centerY = kWindowHeight * 0.5f;
    // Стандартное отклонение (можно менять для более плотного/разрежённого скопления)
    float stddevX = kWindowWidth / 6.0f;
    float stddevY = kWindowHeight / 6.0f;

    std::normal_distribution<float> posXDist(centerX, stddevX);
    std::normal_distribution<float> posYDist(centerY, stddevY);
    std::uniform_real_distribution<float> massDist(1.0f, 1000.0f);
    std::uniform_real_distribution<float> radiusDist(1.0f, 3.0f);

    // Создание объектов 
    std::vector<circle> circles;
    float minX = 90000, maxX = -100, minY = 90000, maxY = -100;
    for (int i = 0; i < 10000; i++) {
         float x = posXDist(gen);
         float y = posYDist(gen);
         float mass = massDist(gen);
         float radius = radiusDist(gen);
         minX = std::min(minX, x);
         minY = std::min(minY, y);
         maxX = std::max(maxX, x);
         maxY = std::max(maxY, y);


         int red = static_cast<int>(255 * (mass - massDist.a()) / (massDist.b() - massDist.a()));
         int green = static_cast<int>(255 * (massDist.b() - mass) / (massDist.b() - massDist.a()));
         sf::Color color(red, red, red);
         circles.emplace_back(std::make_pair(x-radius, y-radius), radius, color, mass, 0.0f , sf::Color::Red);
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

         window.clear(sf::Color::Black);

         // Сброс ускорения у каждого объекта
         for (auto& obj : objects) {
             obj->setAcceleration(0, 0);
         }



         Leaf mainLeaf {minX, minY, maxX, maxY, objects};

         Branch* Quadtree = new Branch(1, window, mainLeaf);
         minX = 90000, maxX = -100, minY = 90000, maxY = -100;



     //     Вычисление гравитационного взаимодействия, обработка столкновений и обновление объектов
         movement::relate(objects, *Quadtree);
         movement::update(objects, dt);


     //     movement::handleCollisions(objects, 0.5f, window);
          for (auto& obj : objects) {
               float x = obj->getPossition().first;
               float y = obj->getPossition().second;

               minX = std::min(minX, x);
               minY = std::min(minY, y);
               maxX = std::max(maxX, x);
               maxY = std::max(maxY, y);
          }




         // Отрисовка объектов
         for (auto& obj : objects) {
              obj->draw(window);
         }

         window.display();
         
    }

    return 0;
}
