#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

//for random
#include <chrono>
#include <random>

#include "building.h"
#include "grid.h"
#include "observer.h"
#include "player.h"
#include "game.h"

int main(int argc, char **argv) {
    std::string board_file_name = "";
    std::string game_file_name = "";
    bool set_seed = false;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-board") {
            i++;
            board_file_name = argv[i];
        }
        if (std::string(argv[i]) == "-seed") {
            set_seed = true;
            i++;
            seed = std::stoi(std::string{argv[i]});
        }
        if (std::string(argv[i]) == "-load") {
            i++;
            game_file_name = argv[i];
        }
    }
    //Grid *g;
    Game *game;
    if (game_file_name != "") {
        std::ifstream infile{game_file_name};
        game = new Game(set_seed, seed, infile, false);
        game->play(false);
    } else if (board_file_name != "") {
        std::ifstream infile{board_file_name};
        game = new Game(set_seed, seed, infile, true);
        game->play(true);
    } else {
        game = new Game(set_seed, seed);
        game->play(true);
    }
    
    delete game;
}
