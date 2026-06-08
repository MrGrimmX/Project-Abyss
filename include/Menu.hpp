#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu();
    // El menú solo necesita comunicarse con el juego a través de eventos y dibujo
    void handleInput(sf::Event event, bool& startRequested, bool& quitRequested);
    void update();
    void draw(sf::RenderWindow& window);

private:
    int selectedOption;
    sf::RectangleShape btnStart, btnExit;
};

#endif