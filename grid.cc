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
#include "tile.h"

void Grid::edge_colour_init(std::unordered_map<size_t, Colour> &map) {
    for (size_t i = 0; i <= max_edge; i++) {
        map[i] = Colour::NoColour;
    }
}

void Grid::node_owner_init(std::unordered_map<size_t, Building *> &map) {
    for (size_t i = 0; i <= max_node; i++) {
        map[i] = nullptr;
    }
}

void Grid::edge_ends_init(std::unordered_map<size_t, std::pair<size_t, size_t>> &map) {
    map[0] = std::make_pair(0, 1);
    map[1] = std::make_pair(0, 3);
    map[2] = std::make_pair(1, 4);
    map[3] = std::make_pair(2, 3);
    map[4] = std::make_pair(4, 5);
    map[5] = std::make_pair(2, 7);
    map[6] = std::make_pair(3, 8);
    map[7] = std::make_pair(4, 9);
    map[8] = std::make_pair(5, 10);
    map[9] = std::make_pair(6, 7);
    map[10] = std::make_pair(8, 9);
    map[11] = std::make_pair(10, 11);
    for (size_t i = 12; i <= 17; i++) {
        map[i] = std::make_pair(i - 6, i);
    }
    map[18] = std::make_pair(13, 14);
    map[19] = std::make_pair(15, 16);
    for (size_t i = 20; i <= 25; i++) {
        map[i] = std::make_pair(i - 8, i - 2);
    }
    map[26] = std::make_pair(18, 19);
    map[27] = std::make_pair(20, 21);
    map[28] = std::make_pair(22, 23);
    for (size_t i = 29; i <= 34; i++) {
        map[i] = std::make_pair(i - 11, i - 5);
    }
    map[35] = std::make_pair(25, 26);
    map[36] = std::make_pair(27, 28);
    for (size_t i = 37; i <= 42; i++) {
        map[i] = std::make_pair(i - 13, i - 7);
    }
    map[43] = std::make_pair(30, 31);
    map[44] = std::make_pair(32, 33);
    map[45] = std::make_pair(34, 35);
    for (size_t i = 46; i <= 51; i++) {
        map[i] = std::make_pair(i - 16, i - 10);
    }
    map[52] = std::make_pair(37, 38);
    map[53] = std::make_pair(39, 40);
    for (size_t i = 54; i <= 59; i++) {
        map[i] = std::make_pair(i - 18, i - 12);
    }
    map[60] = std::make_pair(42, 43);
    map[61] = std::make_pair(44, 45);
    map[62] = std::make_pair(46, 47);
    for (size_t i = 63; i <= 66; i++) {
        map[i] = std::make_pair(i - 20, i - 15);
    }
    map[67] = std::make_pair(48, 49);
    map[68] = std::make_pair(50, 51);
    map[69] = std::make_pair(49, 52);
    map[70] = std::make_pair(50, 53);
    map[71] = std::make_pair(52, 53);
}

void Grid::adjacent_edges_init(std::unordered_map<size_t, std::vector<size_t>> &map) {
    map[0] = {0, 1};
    map[1] = {0, 2};
    map[2] = {3, 5};
    map[3] = {1, 3, 6};
    map[4] = {2, 4, 7};
    map[5] = {4, 8};
    map[6] = {9, 12};
    map[7] = {5, 9, 13};
    map[8] = {6, 10, 14};
    map[9] = {7, 10, 15};
    map[10] = {8, 11, 16};
    map[11] = {11, 17};
    map[12] = {12, 20};
    map[13] = {13, 18, 21};
    map[14] = {14, 18, 22};
    map[15] = {15, 19, 23};
    map[16] = {16, 19, 24};
    map[17] = {17, 25};
    map[18] = {20, 26, 29};
    map[19] = {21, 26, 30};
    map[20] = {22, 27, 31};
    map[21] = {23, 27, 32};
    map[22] = {24, 28, 33};
    map[23] = {25, 28, 34};
    map[24] = {29, 37};
    map[25] = {30, 35, 38};
    map[26] = {31, 35, 39};
    map[27] = {32, 36, 40};
    map[28] = {33, 36, 41};
    map[29] = {34, 42};
    map[30] = {37, 43, 46};
    map[31] = {38, 43, 47};
    map[32] = {39, 44, 48};
    map[33] = {40, 44, 49};
    map[34] = {41, 45, 50};
    map[35] = {42, 45, 51};
    map[36] = {46, 54};
    map[37] = {47, 52, 55};
    map[38] = {48, 52, 56};
    map[39] = {49, 53, 57};
    map[40] = {50, 53, 58};
    map[41] = {51, 59};
    map[42] = {54, 60};
    map[43] = {55, 60, 63};
    map[44] = {56, 61, 64};
    map[45] = {57, 61, 65};
    map[46] = {58, 62, 66};
    map[47] = {59, 62};
    map[48] = {63, 67};
    map[49] = {64, 67, 69};
    map[50] = {65, 68, 70};
    map[51] = {66, 68};
    map[52] = {69, 71};
    map[53] = {70, 71};
}

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

    for (int i = 0; i < max_node; i++) {
        if (valid_building(Colour::Red, i))
            std::cout << "R " << i << std::endl;
        if (valid_building(Colour::Orange, i))
            std::cout << "O " << i << std::endl;
    }
    this->print_grid();
}

Grid::Grid() : goose_tile{nullptr} {
    edge_colour = std::unordered_map<size_t, Colour>();
    node_owner = std::unordered_map<size_t, Building *>();
    edge_ends = std::unordered_map<size_t, std::pair<size_t, size_t>>();
    adjacent_edges = std::unordered_map<size_t, std::vector<size_t>>();
    edge_colour_init(edge_colour);
    node_owner_init(node_owner);
    edge_ends_init(edge_ends);
    adjacent_edges_init(adjacent_edges);
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
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
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

    int park_roll = dice_rolls.at(park_index);
    dice_rolls[seven_index] = park_roll;

    for (size_t i = 0; i <= max_tile; i++) {
        tiles.emplace_back(new Tile(tile_res.at(i), dice_rolls.at(i), false));
    }
}

Grid::Grid(std::ifstream &f) : goose_tile{nullptr} {
    edge_colour = std::unordered_map<size_t, Colour>();
    node_owner = std::unordered_map<size_t, Building *>();
    edge_ends = std::unordered_map<size_t, std::pair<size_t, size_t>>();
    adjacent_edges = std::unordered_map<size_t, std::vector<size_t>>();
    edge_colour_init(edge_colour);
    node_owner_init(node_owner);
    edge_ends_init(edge_ends);
    adjacent_edges_init(adjacent_edges);

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
        tiles.emplace_back(new Tile(res, dice_roll, false));
    }
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
        if (build->get_type() == Building_Type::Basement) {
            std::cout << "B";
        } else if (build->get_type() == Building_Type::House) {
            std::cout << "H";
        } else if (build->get_type() == Building_Type::Tower) {
            std::cout << "T";
        }
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

void Grid::print_grid() const {
    size_t edge_counter = 0;
    size_t node_counter = 0;
    size_t tile_num_counter = 0;
    size_t tile_dice_counter = 0;
    size_t tile_res_counter = 0;

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
    print_tile_break(4);
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
        print_tile_break(6);
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
        // sixth line -> same ase second
        print_tile_break(6);
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
    print_tile_break(4);
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
    print_tile_break(2);
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

bool Grid::valid_upgrade(size_t node_id) {
    if (node_owner[node_id]->get_type() == Building_Type::NoBuilding ||
        node_owner[node_id]->get_type() == Building_Type::Tower) return false;
    return true;
}

void Grid::build_road(Colour colour, size_t edge_id) {
    edge_colour[edge_id] = colour;
}

void Grid::build_building(Player *player, size_t node_id) {
    Building *b = new Building(player);
    node_owner[node_id] = b;
}

void Grid::upgrade_building(size_t node_id) {
    if (!node_owner[node_id]) return;  // no building exists
    node_owner[node_id]->upgrade();
}

Grid::~Grid() {
    if (goose_tile) {
        delete goose_tile;
    }
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

bool Grid::valid_building(Colour player, size_t node_id) {
    /*
     Rules:
     • A residence may not be built on a vertex that is adjacent to a vertex with an existing residence.
     • It is either the beginning of the game, in which case a residence can be built on any vertex, or they have built a road that
       is adjacent to the vertex.

      check if there is an edge of the same color and if that edge does not already connect to a building
    */
    for (auto u : adjacent_edges[node_id]) {
        if (edge_colour[u] == player &&
            !(node_owner[edge_ends[u].first]) &&
            !(node_owner[edge_ends[u].second]))
            return true;
    }

    return false;
}

bool Grid::valid_road(Colour player, size_t edge_id) {
    if (edge_colour[edge_id] != Colour::NoColour)
        return false;

    // we require an adjacent road or vertex

    // road case
    for (auto u : adjacent_edges[edge_ends[edge_id].first])
        if (edge_colour[u] == player)
            return true;

    for (auto u : adjacent_edges[edge_ends[edge_id].second])
        if (edge_colour[u] == player)
            return true;

    // vertex case
    Building *end_point_1 = node_owner[edge_ends[edge_id].first];
    Building *end_point_2 = node_owner[edge_ends[edge_id].first];

    if (end_point_1)
        if (end_point_1->get_Owner()->get_Colour() == player)
            return true;

    if (end_point_2)
        if (end_point_2->get_Owner()->get_Colour() == player)
            return true;

    return false;
}