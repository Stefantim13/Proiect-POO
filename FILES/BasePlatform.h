#ifndef BASE_PLATFORM_H
#define BASE_PLATFORM_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

class BasePlatform {
protected:
    double length, width;
    double x, y;
    sf::RectangleShape plat;
    bool isDestroyed;
    double velocity;

public:
    BasePlatform(double x_, double y_);
    virtual ~BasePlatform() = default;

    virtual void move() = 0;
    virtual void destroy() = 0;
    virtual double propulsiveJumpMultiplier() const = 0;
    virtual std::unique_ptr<BasePlatform> clone() const = 0;
    void draw(sf::RenderWindow &window) const;

    bool getIsDestroyed() const;
    double getX() const;
    double getY() const;
    sf::FloatRect getBounds() const;

    friend std::ostream& operator<<(std::ostream& os ,const BasePlatform& p);

    static int totalPlatforms;
    static int getTotalPlatforms();
};

#endif
