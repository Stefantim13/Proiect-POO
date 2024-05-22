#ifndef DOODLE_H
#define DOODLE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Platform.h"

class Doodle {
    double dimension_x, dimension_y;
    double gravity;
    double jump;
    double x, y;
    double baseJumpStrength;
    sf::RectangleShape rect;

public:
    Doodle();
    void draw(sf::RenderWindow &window);
    void update(std::vector<Platform> &platforms);
    bool checkCollision(const Platform &platform);
    double getX();
    double getY();
    friend std::ostream& operator<<(std::ostream& os, const Doodle& d);
};

#endif
