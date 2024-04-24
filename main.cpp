#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <compare>
#include <chrono>
class Camera
{
    sf::View view;
    double currentY = 0;

public:
    explicit Camera(const sf::Vector2u &windowSize)
    {
        view.setSize(static_cast<double>(windowSize.x), static_cast<double>(windowSize.y));
        view.setCenter(view.getSize() / 2.0f);
    }

    void update(const sf::Vector2f &playerPosition)
    {
        currentY = std::min(playerPosition.y, view.getCenter().y);

        view.setCenter(view.getCenter().x, currentY);
    }
    const sf::View &getView() const
    {
        return view;
    }

    friend std::ostream& operator<<(std::ostream& os, const Camera& cam) {
        os << "Camera [Center: (" << cam.view.getCenter().x << ", " << cam.view.getCenter().y << ")]";
        return os;
    }
};

class Platform
{

    double length = 100, width = 20;
    double x, y;
    int type; /// 0 - infinite; 1 - Destructible; 2 - Moving; 3 - Propulsive
    sf::RectangleShape plat;
    bool isDestroyed = false;
    double velocity = 0;

public:
    Platform(double x_, double y_, int type_) : x(x_), y(y_), type(type_)
    {
        plat.setSize(sf::Vector2f(length, width));
        plat.setPosition(x, y);
        plat.setFillColor(sf::Color::Green);

        if (type == 2)
        {
            velocity = 5;
            plat.setFillColor(sf::Color::Yellow);
        }
        if (type == 3)
        {
            plat.setFillColor(sf::Color::Blue);
        }
        if (type == 1)
        {
            plat.setFillColor(sf::Color::White);
        }
    }


    void move()
    {
        if (type == 2)
        {
            x += velocity;
            if (x < 0 || x + length > 800)
            {
                velocity = -velocity;
            }
            plat.setPosition(x, y);
        }
    }


    void destroy()
    {
        if (type == 1)
        {
            isDestroyed = true;
        }
    }

    bool isDestroyed_()
    {
        return isDestroyed;
    }

    void draw(sf::RenderWindow &window)
    {
        if (!isDestroyed)
        {
            window.draw(plat);
        }
    }

    bool isDestructible() const
    {
        return type == 1;
    }
    double propulsiveJumpMultiplier() const
    {
        if (type == 3)
        {
            return 2.0;
        }
        return 1.0;
    }
    double getX() { return x; }
    double getY() { return y; }
    sf::FloatRect getBounds() const { return plat.getGlobalBounds(); }

    friend std::ostream& operator<<(std::ostream& os ,const Platform& p) {
        os << "Platform [x: " << p.x << ", y: " << p.y << ", type: " << p.type << ", isDestroyed: " << p.isDestroyed << "]";
        return os;
    }
};

class Doodle
{

    double dimension_x = 50, dimension_y = 100;
    double gravity = 0.5;
    double jump = 0;
    double x = 275, y = 800;
    double baseJumpStrength = -16;
    sf::RectangleShape rect;

public:
    Doodle()
    {
        rect.setSize(sf::Vector2f(dimension_x, dimension_y));
        rect.setFillColor(sf::Color::Red);
        rect.setPosition(x, y);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(rect);
    }

    void update(std::vector<Platform> &platforms)
    {
        
        jump += gravity;
        y += jump;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            x += 15;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            x -= 15;
        }

        for (auto &platform : platforms)
        {
            platform.move();
            if (checkCollision(platform) &&  !platform.isDestroyed_())
            {
                if (platform.isDestructible())
                {
                    platform.destroy();
                }
                double multiplier = platform.propulsiveJumpMultiplier();
                jump = baseJumpStrength * multiplier;
                break;
            }
        }

        rect.setPosition(x, y);
    }

    bool checkCollision(const Platform &platform)
    {
        sf::FloatRect doodleBounds = rect.getGlobalBounds();
        sf::FloatRect platformBounds = platform.getBounds();
        return jump > 0 && doodleBounds.intersects(platformBounds);
    }

    double getX()
    {
        return x;
    }

    double getY()
    {
        return y;
    }
    friend std::ostream& operator<<(std::ostream& os,const Doodle& d) {
        os << "Doodle [x: " << d.x << ", y: " << d.y << ", jump: " << d.jump << "]";
        return os;
    }
};

class Game
{
    Doodle doodle;
    sf::RenderWindow &window;
    Camera camera;
    std::vector<Platform> platforms;
    int verticalSpacing = 200;
    int platformCount = 1000;
    int type = 0;
public:
    explicit Game(sf::RenderWindow &window_) : window(window_), camera(window_.getSize())
    {
        generateInitialPlatforms();
    }
    void generateInitialPlatforms()
    {
        platforms.emplace_back(250, 950, 0);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> type_dist(0, 3);

        for (int i = 1; i < platformCount; ++i)
        {
            double Platform_x = randomX();
            double Platform_y = platforms.back().getY() - verticalSpacing;

            switch (type_dist(gen))
            {
            case 0:
                type = 0;
                break;
            case 1:
                type = 1;
                break;
            case 2:
                type = 2;
                break;
            case 3:
                type = 3;
                break;
            }

            platforms.emplace_back(Platform_x, Platform_y, type);
        }
    }
    double randomX()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, window.getSize().x - 100);
        return dis(gen);
    }

    void drawPlatforms()
    {
        for (auto &platform : platforms)
        {
            platform.draw(window);
        }
    }


    void run()
    {
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            doodle.update(platforms);
            camera.update(sf::Vector2f(doodle.getX(), doodle.getY()));
            window.clear();
            window.setView(camera.getView());
            drawPlatforms();
            doodle.draw(window);
            window.display();
        }
    }

    Game(const Game& other): doodle(other.doodle), window(other.window), camera(other.camera), platforms(other.platforms),
      verticalSpacing(other.verticalSpacing), platformCount(other.platformCount), type(other.type){};

    Game& operator=(const Game& other)
    {
        if (this != &other) 
        {
            doodle = other.doodle;        
            camera = other.camera;
            platforms = other.platforms;
            verticalSpacing = other.verticalSpacing;
            platformCount = other.platformCount;
            type = other.type;
        }
        return *this;
    }

    ~Game(){ std::cout << "Sterge";}

    friend std::ostream& operator<<(std::ostream& os, const Game& game) {
        os << "Game [Number of Platforms: " << game.platforms.size() << ", Current Type: " << game.type << "]";
        return os;
    }   
};

int main()
{
    sf::RenderWindow window;
    window.create(sf::VideoMode({800, 1200}), "Doodle Jump", sf::Style::Default);
    window.setFramerateLimit(60);
    Game game(window);
    game.run();
    return 0;
}