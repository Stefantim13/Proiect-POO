#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <SFML/Graphics.hpp>

class HighScore {
    int currentScore;
    int highScore;
    sf::Font font;
    sf::Text scoreText;
    sf::Text highScoreText;

public:
    HighScore();
    void incrementScore(int points);
    void resetCurrentScore();
    void saveHighScore();
    void loadHighScore();
    void draw(sf::RenderWindow& window);
    int getCurrentScore() const;
};

#endif // HIGHSCORE_H
