#ifndef _GAME_H_
#define _GAME_H_
#include "player.h"
#include "grid.h"

class Game {
    Player *blue;
    Player *red;
    Player *orange;
    Player *yellow;
    
    // seed parameters
    bool set_seed;
    unsigned seed;
    // grid is still owned by main depending on how main wants to initialize
    Grid *g; 

    public:
        Game(bool set_seed, unsigned seed, Grid *g);
        void play();
        ~Game();
};

#endif
