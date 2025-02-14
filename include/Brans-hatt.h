#ifndef BARNS_H
#define BARNS_H


// Структура Leaf хранит границы области и вектор объектов в этой области.
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

    // После построения эта ветвь содержит:
    float totalMass;                            ///  ---> суммарная масса всех объектов в узле (либо суммарная масса дочерних узлов)
    float centerX, centerY;                     ///  ---> координаты центра масс

    // Указатели на дочерние ветви (если деление происходило)
    Branch* NW_node = nullptr;
    Branch* NE_node = nullptr;
    Branch* SW_node = nullptr;
    Branch* SE_node = nullptr;

    Leaf myLeaf;                                ///  ---> Копия переданного листа (границы и объекты)


private:
    static constexpr int MaxDepth = 20;         ///  ---> Максимальная глубина рекурсии
    static constexpr size_t Threshold = 10;      ///  ---> Если объектов в Leaf меньше или равно порогу, считаем узел листом

    int level;                                  ///  ---> Текущая глубина
    sf::RenderWindow& window;                   ///  ---> Ссылка на окно для отрисовки (для отладки)

    void FillOut();                             ///  ---> Делит текущий Leaf на 4 квадранта и рекурсивно создаёт дочерние Branch
    void DrawLeaf(const Leaf& leaf);            ///  ---> Отрисовывает границы Leaf (для визуализации)
};

#endif // BARNS_H
