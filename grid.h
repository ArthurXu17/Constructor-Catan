#ifndef _GRID_H_
#define _GRID_H_
#include <vector>
#include <utility>
#include <unordered_map>
#include <iostream>
#include "building.h"
#include "tile.h"


class Grid {
    using size_t = std::size_t;
    const size_t max_edge = 71;
    const size_t max_node = 53;
    const size_t max_tile = 18;
    // for each node -> list of adjacent edges
    std::unordered_map<size_t, std::vector<size_t> > adjacent_edges;

    // for each edge -> gives the pair of nodes 
    std::unordered_map<size_t, std::pair<size_t, size_t>> edge_ends;
    
    // which roads are owned by who
    std::unordered_map<size_t, Colour> edge_colour;
    
    // which vertices are owned by "who" -> this information is in the Building
    std::unordered_map<size_t, Building *> node_owner;

    std::vector<Tile *> tiles;

    Tile *goose_tile;

    Resource int_to_res(int n) const;

    void print_edge(size_t &) const;
    void print_node(size_t &) const;
    void print_tile_break(int n) const;
    void print_resource(Resource r) const;
    void print_tile_edge(size_t &, size_t &) const;
    void print_tile_num(size_t & counter) const;
    void print_tile_dice(size_t & counter) const;
    void print_tile_res(size_t & counter) const;

    public: 
    
    /*std::vector<size_t>& get_edges(size_t idx) {
        return adjacent_edges[idx];
    }*/    
        // constructor for random board generation
        Grid();
        // constructor for file input board generation
        Grid(std::ifstream &f);
        
        void save_game(std::ifstream f);

        // can player PLACE a road here
        bool valid_road(Colour player, size_t edge_id) const;
        // can player PLACE a building here
        bool valid_building(Colour player, size_t node_id) const;
        // function for printing grid into terminal during gameplay

        void build_road(Colour player, size_t edge_id);
        void build_building(Colour player, size_t node_id);
        void print_grid() const;
        void move_goose(); 
        ~Grid();
};

#endif