#ifndef _GAME_H_
#define _GAME_H_
#include "player.h"
#include "grid.h"
#include <fstream>

class Game {
    std::vector<Player*> players;//blue, red, orange, yellow
    int turn;
    // seed parameters
    bool set_seed;
    unsigned seed;
    Grid *g; 
    //dice
    Dice *fair;
    Dice *load;

    public:
        //constructor for random board
        Game(bool set_seed, unsigned seed);
        //constructor for when the board is set
        // case 1: new_game = true, the command line argument was strictly -board
        // case 2: new_game = false, the command line argument was -load
        Game(bool set_seed, unsigned seed, std::ifstream &f, bool new_game);
        void play(bool play_beginning);
        void save_game(std::ofstream &f);
        ~Game();
};

#endif
