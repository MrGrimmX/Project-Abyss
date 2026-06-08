#include "Menu.hpp"

Menu::Menu() : selectedOption(0) {
    // Configura tus rectángulos aquí...
}

void Menu::handleInput(sf::Event event, bool& startRequested, bool& quitRequested) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Down) selectedOption = 1;
        if (event.key.code == sf::Keyboard::Up) selectedOption = 0;
        if (event.key.code == sf::Keyboard::Enter) {
            if (selectedOption == 0) startRequested = true;
            else quitRequested = true;
        }
    }
}

void Menu::draw(sf::RenderWindow& window) {
    // Dibuja tus botones...
}