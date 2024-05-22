#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

class HighScore {
    int currentScore;
    int highScore;
    sf::Font font;
    sf::Text scoreText;
    sf::Text highScoreText;

    void loadHighScore();

public:
    HighScore();
    void incrementScore(int points);
    void saveHighScore();
    void draw(sf::RenderWindow& window);
    int getCurrentScore() const;
};

#endif
