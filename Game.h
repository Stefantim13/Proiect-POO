#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>
#include "Doodle.h"
#include "Camera.h"
#include "Platform.h"
#include "HighScore.h"

class Game {
    Doodle doodle;
    sf::RenderWindow &window;
    Camera camera;
    std::vector<Platform> platforms;
    HighScore highScore;
    int verticalSpacing;
    int platformCount;
    int type;

    enum GameState { Playing, GameOver };
    GameState state;
    sf::Text gameOverText;
    sf::Font font;

    void generateInitialPlatforms();
    double randomX();
    void drawPlatforms();
    void restartGame();

public:
    explicit Game(sf::RenderWindow &window_);
    void run();
    Game(const Game& other);
    Game& operator=(const Game& other);
    ~Game();
    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};

#endif 
