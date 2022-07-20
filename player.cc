#include "player.h"
#include <iostream>

Player::Player(Colour colour): colour{colour} {}

int Player::get_points() const {
    return victory_points;
}

void Player::increment_points() {
    victory_points++;
}

void Player::increment_resource(int index, int amount) {
    resource_count[index]+=amount;
}

bool Player::valid_trade(Player *other, Resource resource_to_give, Resource resource_to_gain) const {
    int give_index = 0;
    if (resource_to_give == Resource::Energy) 
        give_index = 1;
    else if (resource_to_give == Resource::Glass) 
        give_index = 2;
    else if (resource_to_give == Resource::Heat) 
        give_index = 3;
    else if (resource_to_give == Resource::Wifi) 
        give_index = 4;

    if (resource_count.at(give_index) == 0)
        return false;

    int gain_index = 0;
    if (resource_to_gain == Resource::Energy) 
        gain_index = 1;
    else if (resource_to_gain == Resource::Glass) 
        gain_index = 2;
    else if (resource_to_gain == Resource::Heat) 
        gain_index = 3;
    else if (resource_to_gain == Resource::Wifi) 
        gain_index = 4;

    if (other->resource_count.at(gain_index) == 0)
        return false;

    return true;
}

void Player::trade_resources(Player *other, Resource resource_to_give, Resource resource_to_gain) {
    int give_index = 0; // to decrease the resource given
    if (resource_to_give == Resource::Energy) 
        give_index = 1;
    else if (resource_to_give == Resource::Glass) 
        give_index = 2;
    else if (resource_to_give == Resource::Heat) 
        give_index = 3;
    else if (resource_to_give == Resource::Wifi) 
        give_index = 4;

    int gain_index = 0; // to increase the resource gained
    if (resource_to_gain == Resource::Energy) 
        gain_index = 1;
    else if (resource_to_gain == Resource::Glass) 
        gain_index = 2;
    else if (resource_to_gain == Resource::Heat) 
        gain_index = 3;
    else if (resource_to_gain == Resource::Wifi) 
        gain_index = 4;
    
    resource_count[give_index] -= 1; // update exchanged resources
    other->resource_count[give_index] += 1;
    resource_count[gain_index] += 1;
    other->resource_count[gain_index] -= 1;
}

void Player::add_road(size_t edge_id) {
    roads.emplace_back(edge_id);
}

void Player::add_building(size_t node_id, Building_Type building_type) {
    buildings[node_id] = building_type;
    //buildings.emplace_back(std::make_pair(node_id, building_type));
}

void Player::print_colour() const {
    if (colour == Colour::Blue) {
        std::cout<<"Blue ";
    } else if (colour == Colour::Red) {
        std::cout<<"Red ";
    } else if (colour == Colour::Yellow) {
        std::cout<<"Yellow ";
    } else if (colour == Colour::Orange) {
        std::cout<<"Orange ";
    }
}

void Player::print_building_type(Building_Type b) const {
    if (b == Building_Type::Basement) {
        std::cout<<"B ";
    } else if (b == Building_Type::House) {
        std::cout<<"H ";
    } else if (b == Building_Type::Tower) {
        std::cout<<"T ";
    }
}

void Player::print_status() const {

    print_colour();
    std::cout<<"has "<<victory_points<<" building points, ";
    std::cout<<resource_count.at(0)<<" brick, ";
    std::cout<<resource_count.at(1)<<" energy, ";
    std::cout<<resource_count.at(2)<<" glass, ";
    std::cout<<resource_count.at(3)<<" heat, ";
    std::cout<<resource_count.at(4)<<" WiFi."<<std::endl;

    // below code is useful for outputting into file
    /*for (auto x : resource_count) {
        std::cout<<x<<" ";
    }
    std::cout<<"r ";
    for (auto x : roads) {
        std::cout<<x<<" ";
    }
    std::cout<<"h ";
    for (auto kv : buildings) {
        std::cout<<kv.first<<" ";
        if (kv.second == Building_Type::Basement) {
            std::cout<<"B ";
        } else if (kv.second == Building_Type::House) {
            std::cout<<"H ";
        } else if (kv.second == Building_Type::Tower) {
            std::cout<<"T ";
        }
    }
    std::cout<<std::endl;*/
}

void Player::print_buildings() const {
    print_colour();
    std::cout<<"has built:"<<std::endl;
    for (auto kv : buildings) {
        std::cout<<kv.first<<" ";
        print_building_type(kv.second);
        std::cout<<std::endl;
    }
}

Colour Player::get_Colour() const {
    return colour;
}