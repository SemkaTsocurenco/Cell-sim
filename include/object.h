
class OBJ 
{
    private:
        float                          main_size;         ///  --->   Размер главного размера для фигруы 
        float                          BoredrThickness;   ///  --->   Ширина границ объекта 
        sf::Color                      FillColor;         ///  --->   Цвет заливки
        sf::Color                      BorderColor;       ///  --->   Цвет контура 
        std::pair<float, float>        Possition;         ///  --->   Позиция центра 
        std::pair<float, float>        Velocity;          ///  --->   Скорость объекта 
        std::pair<float, float>        Acceleration;      ///  --->   Ускорение объекта 
        const float                    Mass;              ///  --->   Физическая масса объекта
        const float                    Gravity = 500;///  --->   Гравитационная постоянная

    protected:
        sf::CircleShape                CShape;            ///  --->   Форма для круга 
        sf::RectangleShape             RShape;            ///  --->   Форма для квадрата

    public:
        virtual void                   draw(sf::RenderWindow&) = 0 ;           /// Виртуальный метод отрисовки shape
        virtual void                   IncreasePossition(float, float) = 0;    /// Виртуальный метод перемещения объекта

        void                           IncreaseVelocity(float, float);         /// Метод изменения скорости объекта
        void                           IncreaseAcceleration(float, float);     /// Метод изменения ускорения объекта  


        void                           setBorderColor(sf::Color&);
        void                           setFillColor(sf::Color&);
        void                           setBorderThickness(float);
        void                           setMainSize(float);
        void                           setPossition(float, float);
        void                           setVelocity(float, float);
        void                           setAcceleration(float, float);

        void                           updateAcceleration(float dt);
        void                           updateVelocity(float dt);
        void                           updatePosition(float dt);
        void                           relations(OBJ&, float dt);


        std::pair<float, float>&       getPossition();
        std::pair<float, float>&       getVelocity();
        std::pair<float, float>&       getAcceleration();
        float                          getMass();
        float                          getMainSize();


        OBJ (   std::pair <float ,float>   possition,
                float mainsize,
                const sf::Color& fillcolor,
                const float Mass,
                float boredrthickness = 1,
                const sf::Color& bordercolor = sf::Color::Black
            );
        
};


class rectangle : public OBJ
{   
    public: 
        ~rectangle(){}
        sf::RectangleShape             getShape();

        /// @brief Конструктор для квадрата
        ///
        /// @param x                -> Позиция центра x
        /// @param y                -> Позиция центра y
        /// @param mainsize         -> Размер стороны квадрата
        /// @param Mass             -> Масса
        /// @param fillcolor        -> Цвет заливки
        /// @param boredrthickness  -> Ширина контура
        /// @param bordercolor      -> Цвет контура

        rectangle ( std::pair <float ,float>    possition,
                    float                       mainsize, 
                    const sf::Color&            fillcolor,  
                    const float                 Mass,
                    float                       boredrthickness = 1,
                    const sf::Color&            bordercolor = sf::Color::Black
                );
        void draw(sf::RenderWindow&) override; // Переопределяем метод draw
        void IncreasePossition(float, float) override; // Переопределяем метод IncreesePossition


};

class circle : public OBJ
{   
    public: 
        ~circle(){}
        sf::CircleShape                getShape();


        /// @brief Конструктор для квадрата
        ///
        /// @param possition        -> Позиция центра
        /// @param mainsize         -> Радуис
        /// @param Mass             -> Масса
        /// @param fillcolor        -> Цвет заливки
        /// @param boredrthickness  -> Ширина контура
        /// @param bordercolor      -> Цвет контура


        circle   (  std::pair <float ,float>    possition, 
                    float                       mainsize, 
                    const sf::Color&            fillcolor,  
                    const float                 Mass,
                    float                       boredrthickness = 1,
                    const sf::Color&            bordercolor = sf::Color::Black
                );
        void draw(sf::RenderWindow&) override; // Переопределяем метод draw
        void IncreasePossition(float, float) override; // Переопределяем метод IncreesePossition
};


// Класс для обработки движений/взаимодействий
class movement {
    protected:
        static void draw_relatives(const std::pair<float, float>& , const std::pair<float, float>& , float , sf::RenderWindow& );

    public:
        bool drawInteractionLines = true;

        // Сделаем метод статическим и принимающим векторы указателей на OBJ
        static void relate(std::vector<OBJ*>& , float , float , sf::RenderWindow& );
        static void handleCollisions(std::vector<OBJ*>& , float , sf::RenderWindow& );
        void update(std::vector<OBJ*>& , float );
};