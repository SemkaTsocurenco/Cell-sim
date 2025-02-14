#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>  
#include <SFML/System.hpp>
#include <vector>
#include <random>
#include "object.h"
#include "Brans-hatt.h"
#include "movement.h"


struct KEYS {
    sf::Keyboard::Key keyLeft = sf::Keyboard::A;
    sf::Keyboard::Key keyRight = sf::Keyboard::D;
    sf::Keyboard::Key keyUp = sf::Keyboard::W;
    sf::Keyboard::Key keyDown = sf::Keyboard::S;
    sf::Keyboard::Key arrowLeft = sf::Keyboard::Left;
    sf::Keyboard::Key arrowRight = sf::Keyboard::Right;
    sf::Keyboard::Key arrowUp = sf::Keyboard::Up;
    sf::Keyboard::Key arrowDown = sf::Keyboard::Down;

} ;