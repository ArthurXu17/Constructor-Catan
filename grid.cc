#include "grid.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "building.h"
#include "grid_constants.h"
#include "tile.h"

void Grid::test_map() {
    /*std::cout<<"Map edges to neighbouring nodes:"<<std::endl;
    for (auto & kv : edge_ends) {
        std::cout<<kv.first<<": "<<kv.second.first<<", "<<kv.second.second<<std::endl;
    }
    std::cout<<"Map node to adjacent edges:"<<std::endl;
    for (auto & kv : adjacent_edges) {
        std::cout<<kv.first<<": ";
        for (auto x : kv.second) {
            std::cout<<x<<", ";
        }
        std::cout<<std::endl;
    }*/

    std::cout << "Map node to adjacent tiles:" << std::endl;
    for (auto &kv : adjacent_tiles) {
        std::cout << kv.first << ": ";
        for (auto x : kv.second) {
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }

    for (size_t i = 0; i <= max_edge; i++) {
        if (valid_road(Colour::Red, i))
            std::cout << "R " << i << std::endl;
        if (valid_road(Colour::Orange, i))
            std::cout << "O " << i << std::endl;
    }
    this->print_grid();
}

Grid::Grid(bool set_seed_input, unsigned seed_input) : set_seed{set_seed_input}, seed{seed_input} {
    edge_colour = std::unordered_map<size_t, Colour>();
    node_owner = std::unordered_map<size_t, Building *>();
    edge_ends = std::unordered_map<size_t, std::pair<size_t, size_t>>();
    adjacent_edges = std::unordered_map<size_t, std::vector<size_t>>();
    adjacent_tiles = std::unordered_map<size_t, std::vector<size_t>>();
    edge_colour_init(edge_colour);
    node_owner_init(node_owner);
    edge_ends_init(edge_ends);
    adjacent_edges_init(adjacent_edges);
    adjacent_tiles_init(adjacent_tiles);
    /*for (size_t i = 0; i <= max_edge; i++) {
        edge_colour[i] = Colour::NoColour;
    }
    for (size_t i = 0; i <= max_node; i++) {
        node_owner[i] = nullptr;
    }*/

    tiles = std::vector<Tile *>();

    // for random generation of dicerolls and resource tiles
    std::vector<int> dice_rolls = {2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    std::vector<Resource> tile_res = {
        Resource::Brick, Resource::Brick, Resource::Brick, Resource::Brick,
        Resource::Energy, Resource::Energy, Resource::Energy, Resource::Energy,
        Resource::Glass, Resource::Glass, Resource::Glass, Resource::Glass,
        Resource::Heat, Resource::Heat, Resource::Heat,
        Resource::Wifi, Resource::Wifi, Resource::Wifi,
        Resource::Park};

    // use a time-based seed for the default seed value
    if (!set_seed) {
        seed = std::chrono::system_clock::now().time_since_epoch().count();
    }
    std::default_random_engine rng{seed};
    std::shuffle(dice_rolls.begin(), dice_rolls.end(), rng);
    std::shuffle(tile_res.begin(), tile_res.end(), rng);

    size_t seven_index;
    size_t park_index;

    for (size_t i = 0; i < dice_rolls.size(); i++) {
        if (dice_rolls.at(i) == 7) {
            seven_index = i;
        }
        if (tile_res.at(i) == Resource::Park) {
            park_index = i;
        }
    }
    std::swap(dice_rolls.at(park_index), dice_rolls.at(seven_index));
    for (size_t i = 0; i <= max_tile; i++) {
        // std::cout<<"Resource: "<<static_cast<int>(tile_res.at(i))<<", Dice Roll: "<<dice_rolls.at(i)<<std::endl;
        tiles.emplace_back(new Tile(tile_res.at(i), dice_rolls.at(i), tile_res.at(i) == Resource::Park));
    }
}

Grid::Grid(std::ifstream &f) {
    edge_colour = std::unordered_map<size_t, Colour>();
    node_owner = std::unordered_map<size_t, Building *>();
    edge_ends = std::unordered_map<size_t, std::pair<size_t, size_t>>();
    adjacent_edges = std::unordered_map<size_t, std::vector<size_t>>();
    adjacent_tiles = std::unordered_map<size_t, std::vector<size_t>>();
    edge_colour_init(edge_colour);
    node_owner_init(node_owner);
    edge_ends_init(edge_ends);
    adjacent_edges_init(adjacent_edges);
    adjacent_tiles_init(adjacent_tiles);

    /*for (size_t i = 0; i <= max_edge; i++) {
        edge_colour[i] = Colour::NoColour;
    }
    for (size_t i = 0; i <= max_node; i++) {
        node_owner[i] = nullptr;
    }*/

    tiles = std::vector<Tile *>();
    for (int i = 0; i < 19; i++) {
        // read in resource
        int res_int;
        Resource res;
        int dice_roll;
        f >> res_int >> dice_roll;
        res = static_cast<Resource>(res_int);
        tiles.emplace_back(new Tile(res, dice_roll, res == Resource::Park));
    }
}

void Grid::help() const {
    std::cout << "Valid commands: " << std::endl;
    std::cout << "board" << std::endl;
    std::cout << "status" << std::endl;
    std::cout << "residences" << std::endl;
    std::cout << "build-road <edge#>" << std::endl;
    std::cout << "build-red <housing#>" << std::endl;
    std::cout << "improve <housing#>" << std::endl;
    std::cout << "trade <colour> <give> <take>" << std::endl;
    std::cout << "next" << std::endl;
    std::cout << "save <file>" << std::endl;
    std::cout << "help" << std::endl;
}

void Grid::print_edge(size_t &n) const {
    Colour colour = edge_colour.at(n);
    if (colour == Colour::NoColour) {
        std::cout << std::setw(2) << n;
    } else if (colour == Colour::Blue) {
        std::cout << " B";
    } else if (colour == Colour::Red) {
        std::cout << " R";
    } else if (colour == Colour::Yellow) {
        std::cout << " Y";
    } else if (colour == Colour::Orange) {
        std::cout << " O";
    }
    n++;
}

void Grid::print_node(size_t &n) const {
    Building *build = node_owner.at(n);
    std::cout << "|";
    if (build == nullptr) {
        std::cout << std::setw(2) << n;
    } else {
        // print colour
        if (build->get_Owner()->get_Colour() == Colour::Blue) {
            std::cout << "B";
        } else if (build->get_Owner()->get_Colour() == Colour::Red) {
            std::cout << "R";
        } else if (build->get_Owner()->get_Colour() == Colour::Orange) {
            std::cout << "O";
        } else if (build->get_Owner()->get_Colour() == Colour::Yellow) {
            std::cout << "Y";
        }
        // print type of building
        std::cout << build->get_type();
    }
    std::cout << "|";
    n++;
}

void Grid::print_tile_break(int n) const {
    const std::string WHITESPACE = "       ";  // 7 spaces
    for (int i = 0; i < n; i++) {
        std::cout << "  |" << WHITESPACE;
    }
}

void Grid::print_resource(Resource r) const {
    if (r == Resource::Brick) {
        std::cout << " BRICK";
    } else if (r == Resource::Energy) {
        std::cout << "ENERGY";
    } else if (r == Resource::Glass) {
        std::cout << " GLASS";
    } else if (r == Resource::Heat) {
        std::cout << " HEAT ";
    } else if (r == Resource::Park) {
        std::cout << " PARK ";
    } else if (r == Resource::Wifi) {
        std::cout << " WIFI ";
    }
}

void Grid::print_tile_edge(size_t &node_counter, size_t &edge_counter) const {
    print_node(node_counter);
    std::cout << "--";
    print_edge(edge_counter);
    std::cout << "--";
    print_node(node_counter);
}

void Grid::print_tile_num(size_t &counter) const {
    std::cout << std::setw(5) << counter << "   ";
    counter++;
}

void Grid::print_tile_dice(size_t &counter) const {
    if (tiles.at(counter)->get_resource() != Resource::Park) {
        std::cout << std::setw(4) << tiles.at(counter)->get_dice() << "  ";
    } else {
        std::cout << "      ";  // six spaces
    }
    counter++;
}

void Grid::print_tile_res(size_t &counter) const {
    print_resource(tiles.at(counter)->get_resource());
    counter++;
}

void Grid::print_possible_goose(size_t &counter) const {
    if (tiles.at(counter)->getGooseStatus()) {
        std::cout << "  |  GEESE";
        print_tile_break(1);
    } else {
        print_tile_break(2);
    }
    counter++;
}

void Grid::print_grid() const {
    size_t edge_counter = 0;
    size_t node_counter = 0;
    size_t tile_num_counter = 0;
    size_t tile_dice_counter = 0;
    size_t tile_res_counter = 0;
    size_t tile_goose_counter = 0;

    const std::string WHITESPACE = "          ";  // 10 spaces
    const std::string TILE_BREAK = "        ";    // 8 spaces
    std::cout << WHITESPACE << WHITESPACE;
    print_tile_edge(node_counter, edge_counter);
    std::cout << std::endl;
    std::cout << WHITESPACE << WHITESPACE;
    print_tile_break(2);
    std::cout << std::endl;
    std::cout << WHITESPACE << WHITESPACE << " ";
    print_edge(edge_counter);
    print_tile_num(tile_num_counter);
    print_edge(edge_counter);
    std::cout << std::endl;
    std::cout << WHITESPACE << WHITESPACE << "  | ";
    print_tile_res(tile_res_counter);
    std::cout << "  |" << std::endl;
    std::cout << WHITESPACE;
    print_tile_edge(node_counter, edge_counter);
    print_tile_dice(tile_dice_counter);
    print_tile_edge(node_counter, edge_counter);
    std::cout << std::endl
              << WHITESPACE;
    print_tile_break(1);
    print_possible_goose(tile_goose_counter);
    print_tile_break(1);
    std::cout << std::endl
              << WHITESPACE << " ";
    print_edge(edge_counter);
    print_tile_num(tile_num_counter);
    print_edge(edge_counter);
    std::cout << TILE_BREAK;
    print_edge(edge_counter);
    print_tile_num(tile_num_counter);
    print_edge(edge_counter);
    std::cout << std::endl;
    std::cout << WHITESPACE << "  | ";
    print_tile_res(tile_res_counter);
    print_tile_break(1);
    std::cout << "  | ";
    print_tile_res(tile_res_counter);
    std::cout << "  | " << std::endl;

    for (int i = 0; i < 3; i++) {
        // first line
        print_tile_edge(node_counter, edge_counter);
        print_tile_dice(tile_dice_counter);
        print_tile_edge(node_counter, edge_counter);
        print_tile_dice(tile_dice_counter);
        print_tile_edge(node_counter, edge_counter);
        std::cout << std::endl;
        // second line
        print_tile_break(1);
        print_possible_goose(tile_goose_counter);
        print_possible_goose(tile_goose_counter);
        print_tile_break(1);
        std::cout << std::endl;
        // third line
        std::cout << " ";
        print_edge(edge_counter);
        print_tile_num(tile_num_counter);
        print_edge(edge_counter);
        std::cout << TILE_BREAK;
        print_edge(edge_counter);
        print_tile_num(tile_num_counter);
        print_edge(edge_counter);
        std::cout << TILE_BREAK;
        print_edge(edge_counter);
        print_tile_num(tile_num_counter);
        print_edge(edge_counter);
        std::cout << std::endl;
        // fourth line
        std::cout << "  | ";
        print_tile_res(tile_res_counter);
        print_tile_break(1);
        std::cout << "  | ";
        print_tile_res(tile_res_counter);
        print_tile_break(1);
        std::cout << "  | ";
        print_tile_res(tile_res_counter);
        std::cout << "  | " << std::endl;
        // fifth line
        print_node(node_counter);
        print_tile_dice(tile_dice_counter);
        print_tile_edge(node_counter, edge_counter);
        print_tile_dice(tile_dice_counter);
        print_tile_edge(node_counter, edge_counter);
        print_tile_dice(tile_dice_counter);
        print_node(node_counter);
        std::cout << std::endl;
        // sixth line
        print_possible_goose(tile_goose_counter);
        print_possible_goose(tile_goose_counter);
        print_possible_goose(tile_goose_counter);
        std::cout << std::endl;
        // seventh line
        std::cout << " ";
        print_edge(edge_counter);
        std::cout << TILE_BREAK;
        print_edge(edge_counter);
        print_tile_num(tile_num_counter);
        print_edge(edge_counter);
        std::cout << TILE_BREAK;
        print_edge(edge_counter);
        print_tile_num(tile_num_counter);
        print_edge(edge_counter);
        std::cout << TILE_BREAK;
        print_edge(edge_counter);
        std::cout << std::endl;
        // eighth line
        print_tile_break(1);
        std::cout << "  | ";
        print_tile_res(tile_res_counter);
        print_tile_break(1);
        std::cout << "  | ";
        print_tile_res(tile_res_counter);
        print_tile_break(1);
        std::cout << "  | " << std::endl;
    }
    print_tile_edge(node_counter, edge_counter);
    print_tile_dice(tile_dice_counter);
    print_tile_edge(node_counter, edge_counter);
    print_tile_dice(tile_dice_counter);
    print_tile_edge(node_counter, edge_counter);
    std::cout << std::endl;
    std::cout << WHITESPACE;
    print_possible_goose(tile_goose_counter);
    print_possible_goose(tile_goose_counter);
    std::cout << std::endl
              << WHITESPACE << " ";
    print_edge(edge_counter);
    std::cout << TILE_BREAK;
    print_edge(edge_counter);
    print_tile_num(tile_num_counter);
    print_edge(edge_counter);
    std::cout << TILE_BREAK;
    print_edge(edge_counter);
    std::cout << std::endl;
    std::cout << WHITESPACE;
    print_tile_break(1);
    std::cout << "  | ";
    print_tile_res(tile_res_counter);
    print_tile_break(1);
    std::cout << "  | " << std::endl;
    std::cout << WHITESPACE;
    print_tile_edge(node_counter, edge_counter);
    print_tile_dice(tile_dice_counter);
    print_tile_edge(node_counter, edge_counter);
    std::cout << std::endl;
    std::cout << WHITESPACE << WHITESPACE;
    print_possible_goose(tile_goose_counter);
    std::cout << std::endl;
    std::cout << WHITESPACE << WHITESPACE << " ";
    print_edge(edge_counter);
    std::cout << TILE_BREAK;
    print_edge(edge_counter);
    std::cout << std::endl;
    std::cout << WHITESPACE << WHITESPACE;
    print_tile_break(2);
    std::cout << std::endl;
    std::cout << WHITESPACE << WHITESPACE;
    print_tile_edge(node_counter, edge_counter);
    std::cout << std::endl;
}

bool Grid::valid_upgrade(Colour colour, size_t node_id) const {
    if (node_id > 53) return false;  // out of bounds

    if (node_owner.at(node_id) == nullptr ||                           // no building there
        node_owner.at(node_id)->get_type() == Building_Type::Tower ||  // building is already a tower
        node_owner.at(node_id)->get_Owner()->get_Colour() != colour)   // does not own the building
        return false;
    return true;
}

void Grid::build_road(Player *player, size_t edge_id) {
    // update grid value
    edge_colour[edge_id] = player->get_Colour();
    // update player class
    player->add_road(edge_id);
}

void Grid::build_building(Player *player, size_t node_id) {
    Building *b = new Building(player);
    node_owner[node_id] = b;
    // for every adjacent tile, attach the new building as an observer
    for (auto x : adjacent_tiles.at(node_id)) {
        tiles.at(x)->attach(b);
    }
    // increment victory points for player
    player->increment_points();
    // update player class
    player->add_building(node_id, Building_Type::Basement);
}

void Grid::upgrade_building(Player *player, size_t node_id) {
    node_owner[node_id]->upgrade();
    // update player class, after updating the building
    player->add_building(node_id, node_owner.at(node_id)->get_type());
}

Grid::~Grid() {
    for (auto x : tiles) {
        if (x) {
            delete x;
        }
    }
    for (auto kv : node_owner) {
        if (kv.second) {
            delete kv.second;
        }
    }
}

bool Grid::valid_building(Colour player, size_t node_id, bool starting_buildings) const {
    if (node_id > 53) return false;  // out of bounds

    // check for adjacent residences
    for (auto u : adjacent_edges.at(node_id)) {
        if (node_owner.at(edge_ends.at(u).first) != nullptr ||
            node_owner.at(edge_ends.at(u).second) != nullptr)
            return false;
    }

    if (starting_buildings)  // don't need to check for adjacent roads in the beginning
        return true;

    for (auto u : adjacent_edges.at(node_id)) {
        if (edge_colour.at(u) == player)  // must have adjacent road
            return true;
    }

    return false;
}

bool Grid::valid_road(Colour player, size_t edge_id) const {
    if (edge_colour.at(edge_id) != Colour::NoColour)
        return false;

    // we require an adjacent road or vertex

    // road case
    for (auto u : adjacent_edges.at(edge_ends.at(edge_id).first))
        if (edge_colour.at(u) == player)
            return true;

    for (auto u : adjacent_edges.at(edge_ends.at(edge_id).second))
        if (edge_colour.at(u) == player)
            return true;

    // vertex case
    Building *end_point_1 = node_owner.at(edge_ends.at(edge_id).first);
    Building *end_point_2 = node_owner.at(edge_ends.at(edge_id).second);

    if (end_point_1)
        if (end_point_1->get_Owner()->get_Colour() == player)
            return true;

    if (end_point_2)
        if (end_point_2->get_Owner()->get_Colour() == player)
            return true;

    return false;
}

void Grid::update_by_roll(int roll) {
    // blue, red, orange, yellow
    std::vector<std::vector<int>> resource_gain_counter = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};

    for (auto x : tiles) {
        if (x->get_dice() == roll && x->getGooseStatus() == false) {
            x->notify_observers();
            std::unordered_set<Observer *> observers = x->get_observers();
            for (auto o : observers) {  // for each observer, update resources
                // subtract 1 because 0 index is NoColour
                int colour_index = static_cast<int>(o->get_Owner()->get_Colour()) - 1;
                int resource_index = static_cast<int>(x->get_resource());
                int resources_added = o->get_resource_gain();
                resource_gain_counter[colour_index][resource_index] += resources_added;  // increment resources
            }
        }
    }

    std::vector<std::vector<int>> no_builders_gained = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
    std::vector<int> no_resources_gained = {0, 0, 0, 0, 0};
    if (resource_gain_counter == no_builders_gained) {  // no builders gained resources
        std::cout << "No builders gained resources." << std::endl;
    } else {  // show which players gained which resources
        for (int i = 0; i < 4; i++) {
            std::string player = " Blue ";
            if (i == 1)
                player = " Red ";
            else if (i == 2)
                player = " Orange ";
            else if (i == 3)
                player = " Yellow ";
            if (resource_gain_counter.at(i) != no_resources_gained) {
                std::cout << "Builder" << player << "has gained: ";
                if (resource_gain_counter.at(i).at(0) != 0)
                    std::cout << resource_gain_counter.at(i).at(0) << " Bricks ";
                if (resource_gain_counter.at(i).at(1) != 0)
                    std::cout << resource_gain_counter.at(i).at(1) << " Energy ";
                if (resource_gain_counter.at(i).at(2) != 0)
                    std::cout << resource_gain_counter.at(i).at(2) << " Glass ";
                if (resource_gain_counter.at(i).at(3) != 0)
                    std::cout << resource_gain_counter.at(i).at(3) << " Heat ";
                if (resource_gain_counter.at(i).at(4) != 0)
                    std::cout << resource_gain_counter.at(i).at(4) << " Wifi ";
                std::cout << std::endl;
            }
        }
    }
}

size_t Grid::move_goose() {
    size_t new_geese_loc;
    size_t curr_geese_loc;
    for (size_t i = 0; i <= max_tile; i++) {  // get current geese location
        if (tiles[i]->getGooseStatus()) {
            curr_geese_loc = i;
        }
    }

    std::cout << "Choose where to place the GEESE. ";  // prompt for new location
    while (std::cin >> new_geese_loc) {
        if (new_geese_loc <= max_tile && new_geese_loc != curr_geese_loc)  // must be new tile within range
            break;
        std::cout << "Please select a valid location. ";
    }
    tiles[new_geese_loc]->setGooseStatus(true);  // change geese status
    tiles[curr_geese_loc]->setGooseStatus(false);

    return new_geese_loc;
}

int Grid::who_to_steal_from(size_t geese_loc, Player *curr_player) {
    std::cout << "Builder " << curr_player->get_Colour();  // players that can be stolen from

    // determine who can we steal from
    //  blue, red, orange, yellow
    std::vector<Colour> victim_choices;
    for (auto buildings : tiles.at(geese_loc)->get_observers()) {
        // if a players owns a building on tile geese_loc, set to true
        Player *potential_victim = buildings->get_Owner();
        if (potential_victim->get_total_resource() > 0 &&
            potential_victim != curr_player &&
            std::find(victim_choices.begin(), victim_choices.end(), potential_victim->get_Colour()) == victim_choices.end())
            victim_choices.push_back(potential_victim->get_Colour());
    }

    // no builders own a building on tile geese_loc
    if (victim_choices.size() == 0) {
        std::cout << " has no builders to steal from." << std::endl;
        return -1;
    }

    // print builders that own a building on tile geese_loc
    std::cout << " can choose to steal from";
    for (auto p : victim_choices)
        std::cout << " " << p;
    std::cout << "." << std::endl;

    std::cout << "Choose a builder to steal from. ";
    std::string victim;

    while (true) {
        std::cin >> victim;
        transform(victim.begin(), victim.end(), victim.begin(), toupper);
        if (victim == "BLUE" && std::find(victim_choices.begin(), victim_choices.end(), Colour::Blue) != victim_choices.end()) {  // steal from blue
            return 0;
        } else if (victim == "RED" && std::find(victim_choices.begin(), victim_choices.end(), Colour::Red) != victim_choices.end()) {  // steal from blue
            return 1;
        } else if (victim == "ORANGE" && std::find(victim_choices.begin(), victim_choices.end(), Colour::Orange) != victim_choices.end()) {  // steal from blue
            return 2;
        } else if (victim == "YELLOW" && std::find(victim_choices.begin(), victim_choices.end(), Colour::Yellow) != victim_choices.end()) {  // steal from blue
            return 3;
        } else {
            std::cout << "Please choose a valid player" << std::endl;
        }
    }
}