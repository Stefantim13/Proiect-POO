#ifndef DOODLE_H
#define DOODLE_H

#include <SFML/Graphics.hpp>
#include <deque>
#include <memory>
#include "BasePlatform.h"
#include "HighScore.h"

class Doodle {
    double dimension_x, dimension_y;
    double gravity;
    double jump;
    double x, y;
    double baseJumpStrength;
    sf::RectangleShape rect;
    double highestPoint;

public:
    Doodle();
    void draw(sf::RenderWindow &window) const;
    void update(std::deque<std::unique_ptr<BasePlatform>> &platforms, HighScore &highScore);
    bool checkCollision(const BasePlatform &platform) const;
    double getX() const;
    double getY() const;
    void handlePlatformCollision(std::unique_ptr<BasePlatform> &platform);

    friend std::ostream& operator<<(std::ostream& os,const Doodle& d);
};

#endif
