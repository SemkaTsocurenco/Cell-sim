// Класс для обработки движений/взаимодействий
class movement {
    protected:
        static void draw_relatives(const std::pair<float, float>& , const std::pair<float, float>& , float , sf::RenderWindow& );
        static void calculateReaction (Branch&, OBJ&);
        static void addReactionFromNode(Branch&, OBJ&);

        static void addReactionLeafPairwise(Branch&, OBJ&);
        static void addReactionFromNodeAsSingleMass(Branch&, OBJ&);
        
    public:
        bool drawInteractionLines = true;

        // Сделаем метод статическим и принимающим векторы указателей на OBJ
        static void relate(std::vector<OBJ*>&, Branch&);
        static void handleCollisions(std::vector<OBJ*>& , float , sf::RenderWindow& );
        static void update(std::vector<OBJ*>& , float );
};



namespace {
    constexpr float kWindowWidth = 2000.0f;
    constexpr float kWindowHeight = 2000.0f;
    constexpr float kPI = 3.14159265f;
    constexpr float theta = 0.999f;
    constexpr float G = 0.1;

    // Если отрисовка линий взаимодействия не нужна – отключаем на этапе компиляции:
    constexpr bool kDrawInteractionLines = false;
}
