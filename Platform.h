#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Platform {
    double length, width;
    double x, y;
    int type; // 0 - infinite; 1 - Destructible; 2 - Moving; 3 - Propulsive
    sf::RectangleShape plat;
    bool isDestroyed;
    double velocity;

public:
    Platform(double x_, double y_, int type_);
    void move();
    void destroy();
    bool isDestroyed_();
    void draw(sf::RenderWindow &window);
    bool isDestructible() const;
    double propulsiveJumpMultiplier() const;
    double getX();
    double getY();
    sf::FloatRect getBounds() const;
    friend std::ostream& operator<<(std::ostream& os, const Platform& p);
};

#endif
