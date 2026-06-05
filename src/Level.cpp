#include "Level.hpp"
#include <SFML/Graphics.hpp> // <-- Necesario para sf::Vector2f
#include <vector>

std::vector<Level> getBuiltInLevels() {
    std::vector<Level> allLevels;

    // ==========================================
    // NIVEL 1: Hangar de Inicio (Corregido)
    // ==========================================
    Level lvl1;
    lvl1.name = "Hangar de Inicio";
    lvl1.rows = 6; 
    lvl1.cols = 6;
    lvl1.spawnPoint = sf::Vector2f(96.0f, 96.0f); // Celda [1][1]
    lvl1.grid.resize(6, std::vector<Tile>(6, {0, 0.0f, 1.0f, false}));

    // Muros exteriores perimetrales
    for(int i = 0; i < 6; i++) {
        lvl1.grid[0][i].type = 1; 
        lvl1.grid[8][i].type = 1; 
        lvl1.grid[i][0].type = 1; 
        lvl1.grid[i][8].type = 1; 
    }

    // Geometría interna accesible
    lvl1.grid[2][2] = {2, 0.0f, 1.0f, false}; // Puerta Violeta
    lvl1.grid[3][3] = {9, 0.0f, 1.0f, false}; // Salida Verde Abierta
    
    allLevels.push_back(lvl1);

    // ==========================================
    // NIVEL 2: Sector de Elevadores
    // ==========================================
    Level lvl2;
    lvl2.name = "Sector de Elevadores";
    lvl2.rows = 7; 
    lvl2.cols = 7;
    lvl2.spawnPoint = sf::Vector2f(96.0f, 96.0f);
    lvl2.grid.resize(7, std::vector<Tile>(7, {0, 0.0f, 1.0f, false}));
    
    for(int i = 0; i < 7; i++) {
        lvl2.grid[0][i].type = 1; 
        lvl2.grid[6][i].type = 1; 
        lvl2.grid[i][0].type = 1; 
        lvl2.grid[i][6].type = 1; 
    }

    // Escaleras y relieve tridimensional
    lvl2.grid[2][2] = {0, 0.25f, 1.25f, false}; 
    lvl2.grid[2][3] = {0, 0.50f, 1.50f, false}; 
    lvl2.grid[2][4] = {0, 0.75f, 1.75f, false}; 
    
    // El elevador mecánico dinámico
    lvl2.grid[3][4] = {0, 0.0f, 1.75f, true}; 

    // Plataforma de meta
    lvl2.grid[4][4] = {0, 1.0f, 2.0f, false}; 
    lvl2.grid[5][4] = {9, 0.0f, 1.0f, false}; 

    allLevels.push_back(lvl2);

    return allLevels;
}