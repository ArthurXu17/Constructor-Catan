#include <vector>
#include <utility>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include "building.h"
#include "tile.h"
#include "grid.h"

Grid::Grid(): goose_tile{nullptr} {
    edge_colour = std::unordered_map<size_t, Colour>();
    node_owner = std::unordered_map<size_t, Building *>();

    for (size_t i = 0; i <= max_edge; i++) {
        edge_colour[i] = Colour::NoColour;
    }
    for (size_t i = 0; i <= max_node; i++) {
        node_owner[i] = nullptr;
    }

    tiles = std::vector<Tile *>();

    // for random generation of dicerolls and resource tiles
    std::vector<int> dice_rolls =  {2,3,3,4,4,5,5,6,6,7,8,8,9,9,10,10,11,11,12};
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
    std::shuffle( dice_rolls.begin(), dice_rolls.end(), rng );
    std::shuffle( tile_res.begin(), tile_res.end(), rng);

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


Grid::Grid(std::ifstream &f): goose_tile{nullptr} {
    edge_colour = std::unordered_map<size_t, Colour>();
    node_owner = std::unordered_map<size_t, Building *>();

    for (size_t i = 0; i <= max_edge; i++) {
        edge_colour[i] = Colour::NoColour;
    }
    for (size_t i = 0; i <= max_node; i++) {
        node_owner[i] = nullptr;
    }

    tiles = std::vector<Tile *>();
    for (int i = 0; i < 19; i++) {
        // read in resource
        int res_int;
        Resource res;
        int dice_roll;
        f>>res_int>>dice_roll;
        res = static_cast<Resource>(res_int);
        tiles.emplace_back(new Tile(res, dice_roll, false));
    }
}

void Grid::print_edge(size_t &n) const {
    Colour colour = edge_colour.at(n);
    if (colour == Colour::NoColour) {
        std::cout<<std::setw(2)<<n;
    } else if (colour == Colour::Blue) {
        std::cout<<" B";
    } else if (colour == Colour::Red) {
        std::cout<<" R";
    } else if (colour == Colour::Yellow) {
        std::cout<<" Y";
    } else if (colour == Colour::Orange) {
        std::cout<<" O";
    }
    n++;
}

void Grid::print_node(size_t &n) const{
    Building *build = node_owner.at(n);
    std::cout<<"|";
    if (build == nullptr) {
        std::cout<<std::setw(2)<<n;
    } else {
        // print colour
        if (build->get_Owner()->get_Colour() == Colour::Blue) {
            std::cout<<"B";
        } else if (build->get_Owner()->get_Colour() == Colour::Red) {
            std::cout<<"R";
        } else if (build->get_Owner()->get_Colour() == Colour::Orange) {
            std::cout<<"O";
        } else if (build->get_Owner()->get_Colour() == Colour::Yellow) {
            std::cout<<"Y";
        }
        // print type of building
        if (build->get_type() == Building_Type::Basement) {
            std::cout<<"B";
        } else if (build->get_type() == Building_Type::House) {
            std::cout<<"H";
        } else if (build->get_type() == Building_Type::Tower) {
            std::cout<<"T";
        }
    }
    std::cout<<"|";
    n++;
}

void Grid::print_tile_break(int n) const {
    const std::string WHITESPACE = "       ";//7 spaces
    for (int i = 0; i < n; i++) {
        std::cout<<"  |"<<WHITESPACE;
    }
}

void Grid::print_resource(Resource r) const {
    if (r == Resource::Brick) {
        std::cout<<" BRICK";
    } else if (r == Resource::Energy) {
        std::cout<<"ENERGY";
    } else if (r == Resource::Glass) {
        std::cout<<" GLASS";
    } else if (r == Resource::Heat) {
        std::cout<<" HEAT ";
    } else if (r == Resource::Park) {
        std::cout<<" PARK ";
    } else if (r == Resource::Wifi) {
        std::cout<<" WIFI ";
    }
}

void Grid::print_tile_edge(size_t & node_counter, size_t & edge_counter) const {
    print_node(node_counter);
    std::cout<<"--";
    print_edge(edge_counter);
    std::cout<<"--";
    print_node(node_counter);
}

void Grid::print_tile_num(size_t & counter) const {
    std::cout<<std::setw(5)<<counter<<"   ";
    counter++;
}

void Grid::print_tile_dice(size_t & counter) const {
    if (tiles.at(counter)->get_resource() != Resource::Park) {
        std::cout<<std::setw(4)<<tiles.at(counter)->get_dice()<<"  ";
        
    } else {
        std::cout<<"      "; //six spaces
    }
    counter++;
}

void Grid::print_tile_res(size_t & counter) const {
    print_resource(tiles.at(counter)->get_resource());
    counter++;
}

void Grid::print_grid() const {
    size_t edge_counter = 0;
    size_t node_counter = 0;
    size_t tile_num_counter = 0;
    size_t tile_dice_counter = 0;
    size_t tile_res_counter = 0;

    const std::string WHITESPACE = "          "; //10 spaces
    const std::string TILE_BREAK = "        "; //8 spaces
    std::cout<<WHITESPACE<<WHITESPACE;
    print_tile_edge(node_counter, edge_counter);
    std::cout<<std::endl;
    std::cout<<WHITESPACE<<WHITESPACE;
    print_tile_break(2);
    std::cout<<std::endl;
    std::cout<<WHITESPACE<<WHITESPACE<<" ";
    print_edge(edge_counter);
    print_tile_num(tile_num_counter);
    print_edge(edge_counter);
    std::cout<<std::endl;
    std::cout<<WHITESPACE<<WHITESPACE<<"  | ";
    print_tile_res(tile_res_counter);
    std::cout<<"  |"<<std::endl;
    std::cout<<WHITESPACE;
    print_tile_edge(node_counter, edge_counter);
    print_tile_dice(tile_dice_counter);
    print_tile_edge(node_counter, edge_counter);
    std::cout<<std::endl<<WHITESPACE;
    print_tile_break(4);
    std::cout<<std::endl<<WHITESPACE<<" ";
    print_edge(edge_counter);
    print_tile_num(tile_num_counter);
    print_edge(edge_counter);
    std::cout<<TILE_BREAK;
    print_edge(edge_counter);
    print_tile_num(tile_num_counter);
    print_edge(edge_counter);
    std::cout<<std::endl;
    std::cout<<WHITESPACE<<"  | ";
    print_tile_res(tile_res_counter);
    print_tile_break(1);
    std::cout<<"  | ";
    print_tile_res(tile_res_counter);
    std::cout<<"  | "<<std::endl;

    for (int i = 0; i < 3; i++) {
        //first line
        print_tile_edge(node_counter, edge_counter);
        print_tile_dice(tile_dice_counter);
        print_tile_edge(node_counter, edge_counter);
        print_tile_dice(tile_dice_counter);
        print_tile_edge(node_counter, edge_counter);
        std::cout<<std::endl;
        //second line
        print_tile_break(6);
        std::cout<<std::endl;
        // third line
        std::cout<<" ";
        print_edge(edge_counter);
        print_tile_num(tile_num_counter);
        print_edge(edge_counter);
        std::cout<<TILE_BREAK;
        print_edge(edge_counter);
        print_tile_num(tile_num_counter);
        print_edge(edge_counter);
        std::cout<<TILE_BREAK;
        print_edge(edge_counter);
        print_tile_num(tile_num_counter);
        print_edge(edge_counter);
        std::cout<<std::endl;
        // fourth line
        std::cout<<"  | ";
        print_tile_res(tile_res_counter);
        print_tile_break(1);
        std::cout<<"  | ";
        print_tile_res(tile_res_counter);
        print_tile_break(1);
        std::cout<<"  | ";
        print_tile_res(tile_res_counter);
        std::cout<<"  | "<<std::endl;
        // fifth line
        print_node(node_counter);
        print_tile_dice(tile_dice_counter);
        print_tile_edge(node_counter, edge_counter);
        print_tile_dice(tile_dice_counter);
        print_tile_edge(node_counter, edge_counter);
        print_tile_dice(tile_dice_counter);
        print_node(node_counter);
        std::cout<<std::endl;
        // sixth line -> same ase second
        print_tile_break(6);
        std::cout<<std::endl;
        // seventh line
        std::cout<<" ";
        print_edge(edge_counter);
        std::cout<<TILE_BREAK;
        print_edge(edge_counter);
        print_tile_num(tile_num_counter);
        print_edge(edge_counter);
        std::cout<<TILE_BREAK;
        print_edge(edge_counter);
        print_tile_num(tile_num_counter);
        print_edge(edge_counter);
        std::cout<<TILE_BREAK;
        print_edge(edge_counter);
        std::cout<<std::endl;
        //eighth line
        print_tile_break(1);
        std::cout<<"  | ";
        print_tile_res(tile_res_counter);
        print_tile_break(1);
        std::cout<<"  | ";
        print_tile_res(tile_res_counter);
        print_tile_break(1);
        std::cout<<"  | "<<std::endl;
    }
    print_tile_edge(node_counter, edge_counter);
    print_tile_dice(tile_dice_counter);
    print_tile_edge(node_counter, edge_counter);
    print_tile_dice(tile_dice_counter);
    print_tile_edge(node_counter, edge_counter);
    std::cout<<std::endl;
    std::cout<<WHITESPACE;
    print_tile_break(4);
    std::cout<<std::endl<<WHITESPACE<<" ";
    print_edge(edge_counter);
    std::cout<<TILE_BREAK;
    print_edge(edge_counter);
    print_tile_num(tile_num_counter);
    print_edge(edge_counter);
    std::cout<<TILE_BREAK;
    print_edge(edge_counter);
    std::cout<<std::endl;
    std::cout<<WHITESPACE;
    print_tile_break(1);
    std::cout<<"  | ";
    print_tile_res(tile_res_counter);
    print_tile_break(1);
    std::cout<<"  | "<<std::endl;
    std::cout<<WHITESPACE;
    print_tile_edge(node_counter, edge_counter);
    print_tile_dice(tile_dice_counter);
    print_tile_edge(node_counter, edge_counter);
    std::cout<<std::endl;
    std::cout<<WHITESPACE<<WHITESPACE;
    print_tile_break(2);
    std::cout<<std::endl;
    std::cout<<WHITESPACE<<WHITESPACE<<" ";
    print_edge(edge_counter);
    std::cout<<TILE_BREAK;
    print_edge(edge_counter);
    std::cout<<std::endl;
    std::cout<<WHITESPACE<<WHITESPACE;
    print_tile_break(2);
    std::cout<<std::endl;
    std::cout<<WHITESPACE<<WHITESPACE;
    print_tile_edge(node_counter, edge_counter);
    std::cout<<std::endl;
}

void Grid::build_road(Colour colour, size_t edge_id) {
    edge_colour[edge_id] = colour;
}

void Grid::build_building(Player *player, size_t node_id) {
    Building *b = new Building(player);
    node_owner[node_id] = b;

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
