#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <deque>
#include "Doodle.h"
#include "Camera.h"
#include "HighScore.h"
#include "PlatformTypes.h"

class Game {
    Doodle doodle;
    sf::RenderWindow &window;
    Camera camera;
    std::deque<std::unique_ptr<BasePlatform>> platforms;
    HighScore highScore;
    int verticalSpacing;

    enum GameState { Playing, GameOver };
    GameState state;
    sf::Text gameOverText;
    sf::Font font;

public:
    explicit Game(sf::RenderWindow &window_);
    void generateInitialPlatforms();
    std::unique_ptr<BasePlatform> generateRandomPlatform(double y);
    double randomX() const;
    void drawPlatforms() const;
    void maintainPlatformCount();
    void run();
    Game(const Game& other);
    Game& operator=(Game other);
    friend void swap(Game& first, Game& second);
    void restartGame();
    ~Game();

    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};

#endif 
