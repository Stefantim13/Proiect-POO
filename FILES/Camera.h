#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Camera {
    sf::View view;
    double currentY;

public:
    explicit Camera(const sf::Vector2u &windowSize);
    void update(const sf::Vector2f &playerPosition);
    const sf::View &getView() const;
    double getCurrentY() const;

    friend std::ostream& operator<<(std::ostream& os, const Camera& cam);
};

#endif
