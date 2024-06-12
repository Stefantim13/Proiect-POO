#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#include "BasePlatform.h"

class InfinitePlatform : public BasePlatform {
public:
    InfinitePlatform(double x, double y);
    void move() override;
    void destroy() override;
    double propulsiveJumpMultiplier() const override;
    std::unique_ptr<BasePlatform> clone() const override;
};

class DestructiblePlatform : public BasePlatform {
public:
    DestructiblePlatform(double x, double y);
    void move() override;
    void destroy() override;
    double propulsiveJumpMultiplier() const override;
    std::unique_ptr<BasePlatform> clone() const override;
};

class MovingPlatform : public BasePlatform {
public:
    MovingPlatform(double x, double y);
    void move() override;
    void destroy() override;
    double propulsiveJumpMultiplier() const override;
    std::unique_ptr<BasePlatform> clone() const override;
};

class PropulsivePlatform : public BasePlatform {
public:
    PropulsivePlatform(double x, double y);
    void move() override;
    void destroy() override;
    double propulsiveJumpMultiplier() const override;
    std::unique_ptr<BasePlatform> clone() const override;
};

// noua clasa derivata
class InvisiblePlatform : public BasePlatform {
    sf::Clock clock; 
    bool isVisible = true; 

public:
    InvisiblePlatform(double x, double y);
    void move() override;
    void destroy() override;
    double propulsiveJumpMultiplier() const override;
    std::unique_ptr<BasePlatform> clone() const override;
};

#endif
