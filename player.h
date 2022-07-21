#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include <unordered_map>
#include <utility>
// for enum Resource Class
#include "observer.h"

//for random
#include <chrono>
#include <random>

class Building;
enum class Building_Type{NoBuilding, Basement, House, Tower};
enum class Colour{NoColour, Blue, Red, Orange, Yellow};


class Player {
    int victory_points = 0;
    std::vector<int> resource_count = {0, 0, 0, 0, 0};
    std::vector<std::size_t> roads = std::vector<std::size_t>();
    std::unordered_map<std::size_t, Building_Type> buildings = std::unordered_map<std::size_t, Building_Type>();
    Colour colour;
    void print_colour() const;
    void print_building_type(Building_Type b) const;
    void print_resource(size_t type);
    public:
        Player(Colour colour);
        int get_points() const;
        void increment_points();
        void increment_resource(int index, int amount);
        void lose_resource_to_geese();
        void steal(Player *victim);
        void robbed(Player *robber);
        void trade_resources(Player *other, Resource resource_to_give, Resource resource_to_gain);
        // checks if player possesses the resource they are offering (can they make the trade?)
        bool valid_trade_offer(Resource resource_to_give) const;
        // checks if player possesses the resource being asked for (can they accept the trade?)
        bool valid_trade_acceptance(Resource resource_to_gain) const;
        void print_status() const;
        void print_buildings() const;
        void add_road(size_t edge_id);
        void add_building(size_t node_id, Building_Type building_type);
        Colour get_Colour() const;
        ~Player() = default;
};

#endif
