#include "game.h"

Game::Game(bool set_seed_input, unsigned seed_input, Grid *g_input): 
    set_seed{set_seed_input}, seed{seed_input}, g{g_input} {
        blue = new Player(Colour::Blue, set_seed, seed);
        red = new Player(Colour::Red, set_seed, seed);
        orange = new Player(Colour::Orange, set_seed, seed);
        yellow = new Player(Colour::Yellow, set_seed, seed);
}

void Game::play() {
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
        /*for (int i = 4; i < 10; i++) {
            //blue->resource_count = {i, i, i, i, i};

            for (auto p : players) {
                p->lose_resource_to_geese();
                p->print_status();
            }
                
        }*/
        for (auto p : players) {
            
                p->lose_resource_to_geese();
                p->print_status();
        }
        /*for (int i = 0; i <= 18; i++) {
            g->move_goose();
            g->print_grid();
        }*/
        //std::cout << "Builder <colour1> can choose to steal from [builders]" << std::endl;
    }
}

Game::~Game() {
    delete blue;
    delete red;
    delete orange;
    delete yellow;
}

