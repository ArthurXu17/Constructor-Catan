#include <iostream>
#include <fstream>
#include <string>
#include "grid.h"
#include "player.h"
#include "building.h"
#include "observer.h"

int main(int argc, char** argv) {
    std::string file_name = "";
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-load") {
            i++;
            file_name = argv[i];
        }
    }
    std::ifstream infile{file_name};
    Grid *g;
    if (file_name == "") {
        g = new Grid();
    } else {
        g = new Grid(infile);
    }
    g->print_grid();

    Player *blue = new Player(Colour::Blue);
    Player *red = new Player(Colour::Red);
    Player *orange = new Player(Colour::Orange);
    Player *yellow = new Player(Colour::Yellow);
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
    if (red->valid_trade(blue, Resource::Brick, Resource::Glass)) {
        red->trade_resources(blue, Resource::Brick, Resource::Glass);
        red->print_status();
        blue->print_status();
    }
    if (red->valid_trade(blue, Resource::Wifi, Resource::Wifi)) {
        red->trade_resources(blue, Resource::Wifi, Resource::Wifi);
        red->print_status();
        blue->print_status();
    }
    if (red->valid_trade(blue, Resource::Heat, Resource::Wifi)) {
        red->trade_resources(blue, Resource::Heat, Resource::Wifi);
        red->print_status();
        blue->print_status();
    }
    if (red->valid_trade(blue, Resource::Wifi, Resource::Glass)) {
        red->trade_resources(blue, Resource::Wifi, Resource::Glass);
        red->print_status();
        blue->print_status();
    }

    
    //g->test_map();
    /*g->build_building(red, 9);
    g->print_grid();*/

    // for (size_t i = 1; i < 50; i += 5) {
    //     g->build_road(Colour::Yellow, i);
    //     g->print_grid();
    // }
    delete blue;
    delete red;
    delete orange;
    delete yellow;
    delete g;
}