#include "game.h"
#include "dice.h"
#include <string>
#include <algorithm>

Game::Game(bool set_seed_input, unsigned seed_input, Grid *g_input): 
    set_seed{set_seed_input}, seed{seed_input}, g{g_input} {
        blue = new Player(Colour::Blue, set_seed, seed);
        red = new Player(Colour::Red, set_seed, seed);
        orange = new Player(Colour::Orange, set_seed, seed);
        yellow = new Player(Colour::Yellow, set_seed, seed);
}

void Game::play() {
    g->print_grid(); // starting board
    Player *p; // whose turn
    std::string player;
    for (int i = 0; i < 8; i++) { // get initial basements from players
        if (i == 0 || i == 7) {
            p = blue;
            player = "Blue";
        } else if (i == 1 || i == 6) {
            p = red;
            player = "Red";
        } else if (i == 2 || i == 5) {
            p = orange;
            player = "Orange";
        } else if (i == 3 || i == 4) {
            p = yellow;
            player = "Yellow";
        }
        std::cout << "Builder " << player << ", where do you want to build a basement?" << std::endl;
        size_t node; // building vertex
        std::cin >> node;
        while(!g->valid_building(p->get_Colour(), node, true)) { // invalid building placement
            std::cout << "Invalid command. Please try again." << std::endl;
            std::cout << "Builder " << player << ", where do you want to build a basement?" << std::endl;
            std::cin >> node;
        }
        g->build_building(p, node);
    }
    g->print_grid(); // updated grid

    int turn = 0;
    Dice *fair = new RandomDice();
    Dice *load = new LoadedDice(); 
    Dice *current_dice = fair;
    // end when a player has at least 10 points
    while(true) {
        if (turn % 4 == 0) {
            p = blue;
            player = "Blue";
        } else if (turn % 4 == 1) {
            p = red;
            player = "Red";
        } else if (turn % 4 == 2) {
            p = orange;
            player = "Orange";
        } else if (turn % 4 == 3) {
            p = yellow;
            player = "Yellow";
        }
        std::cout << "Builder " << player << "'s turn." << std::endl;
        p->print_status();

        // Beginning of turn phase
        std::string begin_cmd = "fair";
        size_t roll;
        while (true) {
            std::cout << "Current dice are " << begin_cmd;
            std::cout << ". Enter \"load\" to change current dice to loaded dice, \"fair\" to change current dice to fair dice, or \"roll\" to roll the current dice: ";
            std::string cmd;
            std::cin >> cmd; 
            if (cmd != "fair" && cmd != "load" && cmd != "roll") { // invalid command
                std::cout << "Invalid command. Please try again." << std::endl;
                continue;
            } else if (cmd == "fair") { // fair dice
                begin_cmd = "fair";
                current_dice = fair;
            } else if (cmd == "load") { // loaded dice
                begin_cmd = "fair";
                current_dice = load;
            }
            roll = current_dice->generateNumber(); // roll dice
            std::cout << "You have rolled a " << roll << "." << std::endl;
            g->update_by_roll(roll);
            break;
        }
        
        // During the turn phase
        std::string turn_cmd;
        while (turn_cmd != "next") { // end turn given command "next"
            std::cout << "Game phase. Please enter a command (enter \"help\" to show valid commands): ";
            std::cin >> turn_cmd;

            if (turn_cmd == "board") { // prints current board
                g->print_grid();
            }
            
            else if (turn_cmd == "status") { // prints status of all builders in order
                blue->print_status();
                red->print_status();
                orange->print_status();
                yellow->print_status();
            }
            
            else if (turn_cmd == "residences") { // prints residences the current builder has completed
                p->print_buildings();
            }
            
            else if (turn_cmd == "build-road") { // attempts to build road
                int edge;
                std::cin >> edge;
                if (!g->valid_road(p->get_Colour(), edge)) {
                    std::cout << "Invalid command." << std::endl;
                } else {
                    g->build_road(p, edge);
                    std::cout << "Congrats! You have built a road on edge " << edge << "." << std::endl;
                }
            }
            
            else if (turn_cmd == "build-res") { // attempts to build a basement
                int node;
                std::cin >> node;
                if (!g->valid_building(p->get_Colour(), node, false)) {
                    std::cout << "Invalid command." << std::endl;
                } else {
                    g->build_building(p, node);
                    std::cout << "Congrats! You have built a basement on vertex " << node << ". ";
                    std::cout << "You have received 1 building point." << std::endl;
                }
                if (p->win()) break; // achieved 10 or more points
            }
            
            else if (turn_cmd == "improve") { // attempts to improve residence
                int node;
                std::cin >> node;
                if (!g->valid_upgrade(p->get_Colour(), node)) {
                    std::cout << "Invalid command." << std::endl;
                } else {
                    g->upgrade_building(p, node);
                    std::cout << "Congrats! You have updated your residence on vertex " << node << ". ";
                    std::cout << "You have received 1 additional building point." << std::endl;
                }
                if (p->win()) break; // achieved 10 or more points
            }
            
            else if (turn_cmd == "trade") { // attempts to trade
                std::string other, give, gain;
                std::cin >> other; // other player in trade
                std::cin >> give >> gain; // resources being traded
                transform(other.begin(), other.end(), other.begin(), toupper); // converts to uppercase
                transform(give.begin(), give.end(), give.begin(), toupper); 
                transform(gain.begin(), gain.end(), gain.begin(), toupper); 

                Resource resource_to_give, resource_to_gain; // resources being traded
                if (give == "BRICK") resource_to_give = Resource::Brick;
                else if (give == "ENERGY") resource_to_give = Resource::Energy;
                else if (give == "GLASS") resource_to_give = Resource::Glass;
                else if (give == "HEAT") resource_to_give = Resource::Heat;
                else if (give == "WIFI") resource_to_give = Resource::Wifi;
                if (gain == "BRICK") resource_to_give = Resource::Brick;
                else if (gain == "ENERGY") resource_to_gain = Resource::Energy;
                else if (gain == "GLASS") resource_to_gain = Resource::Glass;
                else if (gain == "HEAT") resource_to_gain = Resource::Heat;
                else if (gain == "WIFI") resource_to_gain = Resource::Wifi;

                if (!p->valid_trade_offer(resource_to_give)) { // does not possess sufficient resources to give
                    std::cout << "You do not have enough resources." << std::endl;
                    break;
                }

                while (true) {
                    std::cout << player << " offers" << other << " one " << give << " for one " << gain << "." << std::endl;
                    std::cout << "Does " << other << " accept this offer? "; // trade offer
                    std::string reply;
                    std::cin >> reply;
                    transform(reply.begin(), reply.end(), reply.begin(), toupper);
                    if (reply != "NO" && reply != "YES") { // neither yes nor no
                        std::cout << "Invalid command. Please try again." << std::endl;
                    } else if (reply == "NO") { // no
                        std::cout << "Trade declined." << std::endl;
                        break;
                    } else { // yes
                        if (other == "BLUE") { // trade with blue
                            if (!blue->valid_trade_acceptance(resource_to_gain)) {
                                std::cout << "You do not have enough resources." << std::endl;
                                break;
                            }
                            p->trade_resources(blue, resource_to_give, resource_to_gain);
                        } else if (other == "RED") { // trade with red
                            if (!red->valid_trade_acceptance(resource_to_gain)) {
                                std::cout << "You do not have enough resources." << std::endl;
                                break;
                            }
                            p->trade_resources(red, resource_to_give, resource_to_gain);
                        } else if (other == "ORANGE") { // trade with orange
                            if (!orange->valid_trade_acceptance(resource_to_gain)) {
                                std::cout << "You do not have enough resources." << std::endl;
                                break;
                            }
                            p->trade_resources(orange, resource_to_give, resource_to_gain);
                        } else if (other == "YELLOW") { // trade with yellow
                            if (!yellow->valid_trade_acceptance(resource_to_gain)) {
                                std::cout << "You do not have enough resources." << std::endl;
                                break;
                            }
                            p->trade_resources(yellow, resource_to_give, resource_to_gain);
                        }
                        break;
                    }
                }
            }

            else if (turn_cmd == "next") { // passes control to next player
                continue;
            }
            
            else if (turn_cmd == "save") { // saves current game state
                
            }
            
            else if (turn_cmd == "help") { // prints out list of commands
                g->help();
            }

            else { // invalid command
                std::cout << "Invalid command." << std::endl;
            }
        }
        
        turn++;
    }

    std::cout << "Congrats! Builder " << player << " has earned 10 points and won the game!";

    current_dice = nullptr; // do we need to delete fair and load???
    // delete fair;
    // delete load;

    /*g->build_road(red, 0);
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
    }*/

    // to be called whenever a 7 is rolled
    // int roll = 7;
    // std::vector<Player *> players = {blue, red, orange, yellow};
    // if (roll == 7) {
        /*for (int i = 4; i < 10; i++) {
            //blue->resource_count = {i, i, i, i, i};

            for (auto p : players) {
                p->lose_resource_to_geese();
                p->print_status();
            }
                
        }*/
        // for (auto p : players) {
            
        //         p->lose_resource_to_geese();
        //         p->print_status();
        // }
        /*for (int i = 0; i <= 18; i++) {
            g->move_goose();
            g->print_grid();
        }*/
        //std::cout << "Builder <colour1> can choose to steal from [builders]" << std::endl;
    //}
}

Game::~Game() {
    delete blue;
    delete red;
    delete orange;
    delete yellow;
}

