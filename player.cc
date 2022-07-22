#include "player.h"

#include <iostream>

Player::Player(Colour colour, bool set_seed_input, unsigned seed_input) : colour{colour}, set_seed{set_seed_input}, seed{seed_input} {}

int Player::get_points() const {
    return victory_points;
}

// get total number of resources
int Player::get_total_resource() const{
    int count = 0;
    for (auto r: resource_count) {
        count += r;
    }
    return count;
}

void Player::increment_points() {
    victory_points++;
}

void Player::increment_resource(int index, int amount) {
    resource_count[index] += amount;
}

void Player::trade_resources(Player *other, Resource resource_to_give, Resource resource_to_gain) {
    if (this == other) { // cannot trade with oneself
        std::cout << "Invalid command. Cannot trade with oneself. " << std::endl;
    } else {
        int give_index = static_cast<int>(resource_to_give);
        int gain_index = static_cast<int>(resource_to_gain);

        resource_count[give_index] -= 1;  // trade resources
        other->resource_count[give_index] += 1;
        resource_count[gain_index] += 1;
        other->resource_count[gain_index] -= 1;
    }
}

bool Player::valid_trade_offer(Resource resource_to_give) const {
    int give_index = static_cast<int>(resource_to_give);
    if (resource_count.at(give_index) == 0)
        return false;
    return true;
}

bool Player::valid_trade_acceptance(Resource resource_to_gain) const {
    int gain_index = static_cast<int>(resource_to_gain);
    if (resource_count.at(gain_index) == 0)
        return false;
    return true;
}

void Player::add_road(size_t edge_id) {
    roads.emplace_back(edge_id);
}

void Player::add_building(size_t node_id, Building_Type building_type) {
    buildings[node_id] = building_type;
    // buildings.emplace_back(std::make_pair(node_id, building_type));
}

void Player::print_colour() const {
    if (colour == Colour::Blue) {
        std::cout << "Blue ";
    } else if (colour == Colour::Red) {
        std::cout << "Red ";
    } else if (colour == Colour::Yellow) {
        std::cout << "Yellow ";
    } else if (colour == Colour::Orange) {
        std::cout << "Orange ";
    }
}

void Player::print_status() const {
    print_colour();
    std::cout << "has " << victory_points << " building points, ";
    std::cout << resource_count.at(0) << " brick, ";
    std::cout << resource_count.at(1) << " energy, ";
    std::cout << resource_count.at(2) << " glass, ";
    std::cout << resource_count.at(3) << " heat, ";
    std::cout << resource_count.at(4) << " WiFi." << std::endl;

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
    std::cout << "has built:" << std::endl;
    for (auto kv : buildings) {
        std::cout << kv.first << " ";
        std::cout<<kv.second;
        //print_building_type(kv.second);
        std::cout << std::endl;
    }
}

Colour Player::get_Colour() const {
    return colour;
}

void Player::lose_resource_to_geese() {
    int total_resources_count = get_total_resource();

    if (total_resources_count >= 10) {
        int half = total_resources_count / 2;

        std::cout << "Builder ";
        this->print_colour();
        std::cout << "loses " << half << " resources to the geese. They lose:" << std::endl;

        // use a time-based seed for the default seed value
        if (!set_seed) {
            seed = std::chrono::system_clock::now().time_since_epoch().count();
        }
        std::mt19937 gen(seed);
        std::uniform_int_distribution<std::mt19937::result_type> dist4(0, 4);
        std::vector<int> resource_lost_count{0, 0, 0, 0, 0};  // purely used to keep track of cumulative amount of each resource lost

        while (half > 0) {
            int resource_to_be_lost = dist4(gen);
            if (resource_count[resource_to_be_lost] > 0) {
                resource_lost_count[resource_to_be_lost]++;
                resource_count[resource_to_be_lost]--;
                half--;
            }
        }
        std::cout << resource_lost_count[0] << " brick " << std::endl;
        std::cout << resource_lost_count[1] << " energy " << std::endl;
        std::cout << resource_lost_count[2] << " glass " << std::endl;
        std::cout << resource_lost_count[3] << " heat " << std::endl;
        std::cout << resource_lost_count[4] << " WiFi." << std::endl;
    }
}

// steals a resource from a player assuming they are valid (they have at least one resource)
void Player::steal(Player *victim) {
    victim->robbed(this);
}

void Player::robbed(Player *robber) {
    if (!set_seed) {
        seed = std::chrono::system_clock::now().time_since_epoch().count();
    }
    std::mt19937 gen(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist4(0, 4);
    while (true) {
        int resource = dist4(gen);
        if (resource_count[resource] > 0) {
            resource_count[resource]--;
            robber->increment_resource(resource, 1);
            std::cout << "Builder " << robber->get_Colour() << " steals " << print_resource(resource) << " from builder " << this->get_Colour() << " ."<< std::endl;
            break;
        }
    }
}

std::string Player::print_resource(size_t type) {
    switch (type) {
        case 0:
            return "Brick";
            break;
        case 1:
            return "Energy";
            break;
        case 2:
            return "Glass";
            break;
        case 3:
            return "Heat";
            break;
        default:
            return "Wifi";
    }
}

bool Player::win() const {
    if (victory_points >= 10) {
        
        return true;
    }
    return false;
}