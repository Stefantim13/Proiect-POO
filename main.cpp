#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <compare>
class Camera
{
    sf::View view;
    double view_speed = 0.1;
    double currentY;

public:
    Camera(const sf::Vector2u &windowSize)
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
    const double length = 100, width = 20;
    int type; /// tip de platforma (rezista la infinit, se distruge dupa ce sari o data pe ea, te propulseaza mai mult in sus...)
    double x, y;
    sf::RectangleShape plat;

public:
    Platform(double x_, double y_, int type_ = 0) : x(x_), y(y_), type(type_)
    {
        plat.setSize(sf::Vector2f(length, width));
        plat.setPosition(x, y);
        plat.setFillColor(sf::Color::Green);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(plat);
    }

    double getX()
    {
        return x;
    }

    double getY()
    {
        return y;
    }

    double getLength()
    {
        return length;
    }

    double getWidth()
    {
        return width;
    }

    sf::FloatRect getBounds() const
    {
        return plat.getGlobalBounds();
    }
};

class Doodle
{
    const double dimension_x = 50, dimension_y = 100;
    const double gravity = 0.5;
    const double jumpStrength = -16;
    double jump = 0;
    double x = 275, y = 800;
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

    void update(const std::vector<Platform> &platforms)
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

        for (const auto &platform : platforms)
        {
            if (checkCollision(platform))
            {
                jump = jumpStrength;
                break;
            }
        }

        rect.setPosition(x, y);
    }

    bool checkCollision(const Platform &platform)
    {
        sf::FloatRect doodleBounds, platformBounds;
        doodleBounds = rect.getGlobalBounds();
        platformBounds = platform.getBounds();

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
    const int platformCount = 100;
    double x, y;

public:
    Game(sf::RenderWindow &window_) : window(window_), camera(window_.getSize())
    {
        generateInitialPlatforms();
    }
    void generateInitialPlatforms()
    {
        platforms.emplace_back(250, 950);

        for (int i = 1; i < platformCount; ++i)
        {
            x = randomX();
            y = platforms[platforms.size()-1].getY() - verticalSpacing;
            platforms.emplace_back(x, y);
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
    window.setVerticalSyncEnabled(true); 
    Game game(window);
    game.run();
    return 0;
}