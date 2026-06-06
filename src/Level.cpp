#include "Level.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

std::vector<Level> getBuiltInLevels() {
    std::vector<Level> allLevels;

    // ==========================================
    // NIVEL 1: El Escape
    // ==========================================
    Level lvl1;
    lvl1.name = "El Escape";
    lvl1.rows = 8; 
    lvl1.cols = 8;
    lvl1.spawnPoint = sf::Vector2f(96.0f, 96.0f); // Celda [1][1]
    lvl1.grid.resize(8, std::vector<Tile>(8, {0})); // Todo vacío por defecto

    // Crear muros exteriores perimetrales
    for(int i = 0; i < 8; i++) {
        lvl1.grid[0][i].type = 1; 
        lvl1.grid[7][i].type = 1; 
        lvl1.grid[i][0].type = 1; 
        lvl1.grid[i][8-1].type = 1; 
    }

    // Geometría interna clásica Wolfenstein
    lvl1.grid[3][2].type = 1;
    lvl1.grid[3][3].type = 2; // Puerta Violeta de paso obligado
    lvl1.grid[3][4].type = 1;
    lvl1.grid[2][1].type = 3; // Muro secreto camuflado en la pared izquierda
    lvl1.grid[2][0].type = 0; // Pasillo oculto exterior por donde se retirará el bloque
    
    lvl1.grid[5][5].type = 9; // Cuadro de Salida Verde al Nivel 2

    allLevels.push_back(lvl1);

    // ==========================================
    // NIVEL 2: Los Laberintos Azules
    // ==========================================
    Level lvl2;
    lvl2.name = "Laberinto";
    lvl2.rows = 8; 
    lvl2.cols = 8;
    lvl2.spawnPoint = sf::Vector2f(96.0f, 96.0f);
    lvl2.grid.resize(8, std::vector<Tile>(8, {0}));
    
    for(int i = 0; i < 8; i++) {
        lvl2.grid[0][i].type = 1; 
        lvl2.grid[7][i].type = 1; 
        lvl2.grid[i][0].type = 1; 
        lvl2.grid[i][7].type = 1; 
    }

    // Paredes del laberinto plano
    lvl2.grid[2][2].type = 1; lvl2.grid[2][3].type = 1; lvl2.grid[2][4].type = 1;
    lvl2.grid[4][4].type = 1; lvl2.grid[5][4].type = 1;
    lvl2.grid[5][5].type = 9; // Salida final del juego

    allLevels.push_back(lvl2);

    return allLevels;
}