#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "Game.cpp" // Incluimos tu clase Game original

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Wolfenstein 3D - Menu");
    Menu menu;
    Game game; // Tu clase Game original
    
    bool playing = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (!playing) {
                // El menú captura el evento
                menu.handleInput(event, playing);
            }
        }

        window.clear();
        if (!playing) {
            menu.draw(window);
        } else {
            // Aquí es donde "rompías" la lógica antes. 
            // Ahora, en lugar de meter el menú en Game, 
            // usamos la ventana de Game o llamamos a game.run()
            game.run(); 
        }
        window.display();
    }
    return 0;
}