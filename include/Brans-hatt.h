#ifndef BARNS_H
#define BARNS_H


struct Leaf {
    float minX;
    float minY;
    float maxX;
    float maxY;
    std::vector<OBJ*> objects;

    Leaf(float minX_, float minY_, float maxX_, float maxY_, const std::vector<OBJ*>& objs)
        : minX(minX_), minY(minY_), maxX(maxX_), maxY(maxY_), objects(objs) {}
};

class Branch {
public:
    Branch(int level, sf::RenderWindow& window, const Leaf& leaf);
    ~Branch() = default;

    void FillOut();
    void DrawLeaf(const Leaf& leaf);
    
private:
    static constexpr int MaxDepth = 20; // Максимальная глубина рекурсии
    int level;
    Leaf myLeaf;
    sf::RenderWindow& window;

    // Эти вектора будут переданы в соответствующие квадранты (инициализированы пустыми)
    std::vector<OBJ*> NWObj;
    std::vector<OBJ*> NEObj;
    std::vector<OBJ*> SWObj;
    std::vector<OBJ*> SEObj;
};

#endif // BARNS_H
