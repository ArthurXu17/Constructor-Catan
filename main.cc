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
        //g = new Grid(set_seed, seed);
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
    //g->print_grid();
    //Game *game = new Game(set_seed, seed, g);
    delete game;
    //delete g;

    /*Player *blue = new Player(Colour::Blue, set_seed, seed);
    Player *red = new Player(Colour::Red, set_seed, seed);
    Player *orange = new Player(Colour::Orange, set_seed, seed);
    Player *yellow = new Player(Colour::Yellow, set_seed, seed);
    g->build_road(red, 0);
    g->build_road(red, 20);
    g->print_grid();
    red->print_status();
    red->print_buildings();
    blue->print_status();
    blue->print_buildings();
    g->build_building(red, 20);
    g->print_grid();
    g->build_building(red, 21);
    g->build_building(blue, 26);
    g->print_grid();
    red->print_status();
    red->print_buildings();
    blue->print_status();
    blue->print_buildings();
    g->update_by_roll(11);
    red->print_status();
    blue->print_status();
    g->update_by_roll(2);
    red->print_status();
    blue->print_status();
    g->update_by_roll(3);
    red->print_status();
    blue->print_status();
    g->upgrade_building(red, 20);
    g->print_grid();
    red->print_buildings();
    blue->print_buildings();
    g->update_by_roll(11);
    red->print_status();
    blue->print_status();
    g->update_by_roll(2);
    red->print_status();
    blue->print_status();
    g->build_building(red, 38);
    g->upgrade_building(red, 38);
    g->upgrade_building(red, 38);
    g->print_grid();
    red->print_status();
    red->print_buildings();
    g->update_by_roll(11);
    red->print_status();
    blue->print_status();
    if (red->valid_trade_offer(Resource::Brick) && blue->valid_trade_acceptance(Resource::Glass)) {
        red->trade_resources(blue, Resource::Brick, Resource::Glass);
        red->print_status();
        blue->print_status();
    }
    if (red->valid_trade_offer(Resource::Wifi) && blue->valid_trade_acceptance(Resource::Wifi)) {
        red->trade_resources(blue, Resource::Wifi, Resource::Wifi);
        red->print_status();
        blue->print_status();
    }
    if (red->valid_trade_offer(Resource::Heat) && blue->valid_trade_acceptance(Resource::Wifi)) {
        red->trade_resources(blue, Resource::Heat, Resource::Wifi);
        red->print_status();
        blue->print_status();
    }
    if (red->valid_trade_offer(Resource::Wifi) && blue->valid_trade_acceptance(Resource::Glass)) {
        red->trade_resources(blue, Resource::Wifi, Resource::Glass);
        red->print_status();
        blue->print_status();
    }

    // to be called whenever a 7 is rolled
    int roll = 7;
    std::vector<Player *> players = {blue, red, orange, yellow};
    if (roll == 7) {*/
        /*for (int i = 4; i < 10; i++) {
            //blue->resource_count = {i, i, i, i, i};

            for (auto p : players) {
                p->lose_resource_to_geese();
                p->print_status();
            }
                
        }*/
        /*for (auto p : players) {
            
                p->lose_resource_to_geese();
                p->print_status();
        }*/
        /*for (int i = 0; i <= 18; i++) {
            g->move_goose();
            g->print_grid();
        }
        std::cout << "Builder <colour1> can choose to steal from [builders]" << std::endl;
    }*/
    // g->test_map();
    /*g->build_building(red, 9);
    g->print_grid();*/

    // for (size_t i = 1; i < 50; i += 5) {
    //     g->build_road(Colour::Yellow, i);
    //     g->print_grid();
    // }
    
}