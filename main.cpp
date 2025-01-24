#include <iostream>
#include "./include/dot.h"



int main(){
    dot dot1 (10., 20.);
    COORD coord = dot1.get();
    std::cout<<coord.x<<"   "<<coord.y<<"\n";

    dot1.vel.V_x = 190.;
    dot1.vel.V_y = -11.;

    std::cout<<dot1.get_V().V_x<<"   "<<dot1.get_V().V_y<<"\n";
}