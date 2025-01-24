

struct COORD
{
    float x;
    float y;
};

struct VEL
{
    float V_x;
    float V_y;
};


class dot {
private:


public:
    COORD coord;
    VEL vel;
    
    dot(float X, float Y){
        coord.x = X;
        coord.y = Y;
        vel = {0,0};
    }

    COORD get(int asix = 0){
        return coord;
    };

    float get_x(){
        return coord.x;
    };
    float get_y(){
        return coord.y;
    };


    float get_V_x(){
        return vel.V_x;
    };

    float get_V_y(){
        return vel.V_y;
    };
    

    VEL get_V(int asix = 0){
        return vel;
    };


};