#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <compare>
#include <chrono>
class Camera
{
    sf::View view;
    double view_speed = 0.1;
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
};

class Platform
{
    friend class Game;

    const double length = 100, width = 20;
    enum PlatformType
    {
        Infinite,
        Destructible,
        Moving,
        Propulsive
    };
    PlatformType type;
    double x, y;
    sf::RectangleShape plat;
    bool isDestroyed = false;
    double velocity = 0;

public:
    Platform(double x_, double y_, PlatformType type_) : x(x_), y(y_), type(type_)
    {
        plat.setSize(sf::Vector2f(length, width));
        plat.setPosition(x, y);
        plat.setFillColor(sf::Color::Green);

        if (type == Moving)
        {
            velocity = 5;
            plat.setFillColor(sf::Color::Yellow);
        }
        if (type == Propulsive)
        {
            plat.setFillColor(sf::Color::Blue);
        }
        if (type == Destructible)
        {
            plat.setFillColor(sf::Color::White);
        }
    }


    void move()
    {
        if (type == Moving)
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
        if (type == Destructible)
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
        return type == Destructible;
    }
    double propulsiveJumpMultiplier() const
    {
        if (type == Propulsive)
        {
            return 2.0;
        }
        return 1.0;
    }
    double getX() { return x; }
    double getY() { return y; }
    double getLength() { return length; }
    double getWidth() { return width; }
    sf::FloatRect getBounds() const { return plat.getGlobalBounds(); }
};

class Doodle
{

    const double dimension_x = 50, dimension_y = 100;
    const double gravity = 0.5;
    const double jumpStrength = -16;
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
};

class Game
{
    Doodle doodle;
    Camera camera;
    std::vector<Platform> platforms;
    sf::RenderWindow &window;
    const int verticalSpacing = 200;
    const int platformCount = 1000;
    double x, y;

public:
    explicit Game(sf::RenderWindow &window_) : window(window_), camera(window_.getSize())
    {
        generateInitialPlatforms();
    }
    void generateInitialPlatforms()
    {
        platforms.emplace_back(250, 950, Platform::Infinite);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> type_dist(0, 3);

        for (int i = 1; i < platformCount; ++i)
        {
            double x = randomX();
            double y = platforms.back().getY() - verticalSpacing;

            Platform::PlatformType type;
            switch (type_dist(gen))
            {
            case 0:
                type = Platform::Infinite;
                break;
            case 1:
                type = Platform::Destructible;
                break;
            case 2:
                type = Platform::Moving;
                break;
            case 3:
                type = Platform::Propulsive;
                break;
            }

            platforms.emplace_back(x, y, type);
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

    std::vector<Platform> &getPlatforms()
    {
        return platforms;
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