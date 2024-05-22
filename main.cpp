#include <SFML/Graphics.hpp>
#include "Game.h"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({800, 1200}), "Doodle Jump", sf::Style::Default);
    window.setFramerateLimit(60);
    Game game(window);
    game.run();
    return 0;
}
