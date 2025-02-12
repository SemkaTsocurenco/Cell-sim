#include "../include/main.h"



void OBJ::setBorderColor(sf::Color& color){
    this -> BorderColor = color;
}

void OBJ::setFillColor(sf::Color& color){
    this -> FillColor = color;
}

void OBJ::setBorderThickness(float thickness){
    this -> BoredrThickness = thickness;
}

void OBJ::setMainSize(float size){
    this -> main_size = size;
}

void OBJ::setPossition(float x, float y){
    this -> Possition.first = x;
    this -> Possition.second = y;
}


void circle::draw(sf::RenderWindow& window) {
    window.draw(this->CShape);
} 

void rectangle::draw(sf::RenderWindow& window) {
    window.draw(this->RShape);
}



std::pair<float, float>&  OBJ::getPossition(){
    return this->Possition;
}

void circle::IncreasePossition (float x , float y){
    setPossition(this->getPossition().first + x ,this->getPossition().second + y );
    this -> CShape.setPosition (this->getPossition().first ,this->getPossition().second );
}


void rectangle::IncreasePossition (float x , float y){
    setPossition(this->getPossition().first + x ,this->getPossition().second + y );
    this -> RShape.setPosition (this->getPossition().first ,this->getPossition().second );
}


sf::CircleShape circle::getShape (){
    return this->CShape;
}

sf::RectangleShape rectangle::getShape (){
    return this->RShape;
}

float OBJ::getMass(){
    return this -> Mass;
}

float OBJ::getMainSize(){
    return this -> main_size;
}



OBJ::OBJ(std::pair<float, float> possition,
         float mainsize,
         const sf::Color& fillcolor,
         const float mass,
         float boredrthickness,
         const sf::Color& bordercolor)
    : Possition(possition),
      main_size(mainsize),
      FillColor(fillcolor),
      Mass (mass),
      BoredrThickness(boredrthickness),
      BorderColor(bordercolor) {
}

rectangle::rectangle(std::pair<float ,float>  possition,
                     float                    mainSize,
                     const sf::Color&         fillColor, 
                     const float              Mass,
                     float                    borderThickness,
                     const sf::Color&         borderColor)  : OBJ(possition,
                                                                  mainSize,
                                                                  fillColor, 
                                                                  Mass,
                                                                  borderThickness, 
                                                                  borderColor) {
    RShape.setFillColor(fillColor);
    RShape.setSize({mainSize, mainSize});
    RShape.setOutlineThickness(borderThickness);
    RShape.setPosition(possition.first, possition.second);
    RShape.setOutlineColor(borderColor);
}

circle::circle(std::pair<float ,float>        possition,
                     float                    mainSize,
                     const sf::Color&         fillColor, 
                     const float              Mass,
                     float                    borderThickness,
                     const sf::Color&         borderColor)  : OBJ(possition,
                                                                  mainSize,
                                                                  fillColor, 
                                                                  Mass,
                                                                  borderThickness, 
                                                                  borderColor) {
    CShape.setFillColor(fillColor);
    CShape.setRadius(mainSize);
    CShape.setOutlineThickness(borderThickness);
    CShape.setPosition(possition.first, possition.second);
    CShape.setOutlineColor(borderColor);
}