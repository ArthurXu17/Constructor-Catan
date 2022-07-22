#include <fstream>
#include <iostream>
#include <string>

#include "building.h"
#include "grid.h"
#include "observer.h"
#include "player.h"

int main(int argc, char **argv) {
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
    if (roll == 7) {
        for (int i = 4; i < 10; i++) {
            //blue->resource_count = {i, i, i, i, i};

            for (auto p : players)
                p->lose_resource_to_geese();
        }
        for (int i = 0; i <= 18; i++) {
            g->move_goose();
            g->print_grid();
        }
        std::cout << "Builder <colour1> can choose to steal from [builders]" << std::endl;
    }
    // g->test_map();
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