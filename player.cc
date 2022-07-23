#include "player.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "resource_costs.h"

Player::Player(Colour colour, bool set_seed_input, unsigned seed_input,
               Dice *dice_input)
    : colour{colour},
      set_seed{set_seed_input},
      seed{seed_input},
      dice{dice_input} {}

int Player::get_points() const { return victory_points; }

// get total number of resources
int Player::get_total_resource() const {
    int count = 0;
    for (auto r : resource_count) {
        count += r;
    }
    return count;
}

std::vector<std::size_t> Player::get_roads() const { return roads; }
std::unordered_map<std::size_t, Building_Type> Player::get_buildings() const {
    return buildings;
}

void Player::increment_points() { victory_points++; }

void Player::increment_resource(int index, int amount) {
    resource_count[index] += amount;
}

void Player::trade_resources(Player *other, Resource resource_to_give,
                             Resource resource_to_gain) {
    if (this == other) {  // cannot trade with oneself
        std::cout << "Invalid command. Cannot trade with oneself. "
                  << std::endl;
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
    if (resource_count.at(give_index) == 0) return false;
    return true;
}

bool Player::valid_trade_acceptance(Resource resource_to_gain) const {
    int gain_index = static_cast<int>(resource_to_gain);
    if (resource_count.at(gain_index) == 0) return false;
    return true;
}

void Player::add_road(size_t edge_id) {
    // only add if edge_id doesn't already exist
    // we need to check this for when we create a game from a loaded state
    bool exist_already = false;
    for (auto x : roads) {
        if (x == edge_id) {
            exist_already = true;
        }
    }
    if (!exist_already) {
        roads.emplace_back(edge_id);
    }
}

void Player::add_building(size_t node_id, Building_Type building_type) {
    buildings[node_id] = building_type;
    // buildings.emplace_back(std::make_pair(node_id, building_type));
}

bool Player::can_buy_road() const {
    for (size_t i = 0; i < resource_count.size(); i++) {
        if (resource_count.at(i) < road_cost.at(i)) {
            return false;
        }
    }
    return true;
}
bool Player::can_buy_basement() const {
    for (size_t i = 0; i < resource_count.size(); i++) {
        if (resource_count.at(i) < basement_cost.at(i)) {
            return false;
        }
    }
    return true;
}
bool Player::can_buy_house() const {
    for (size_t i = 0; i < resource_count.size(); i++) {
        if (resource_count.at(i) < house_cost.at(i)) {
            return false;
        }
    }
    return true;
}
bool Player::can_buy_tower() const {
    for (size_t i = 0; i < resource_count.size(); i++) {
        if (resource_count.at(i) < tower_cost.at(i)) {
            return false;
        }
    }
    return true;
}

void Player::purchase_road() {
    for (size_t i = 0; i < resource_count.size(); i++) {
        resource_count.at(i) -= road_cost.at(i);
    }
}
void Player::purchase_basement() {
    for (size_t i = 0; i < resource_count.size(); i++) {
        resource_count.at(i) -= basement_cost.at(i);
    }
}
void Player::purchase_house() {
    for (size_t i = 0; i < resource_count.size(); i++) {
        resource_count.at(i) -= house_cost.at(i);
    }
}
void Player::purchase_tower() {
    for (size_t i = 0; i < resource_count.size(); i++) {
        resource_count.at(i) -= tower_cost.at(i);
    }
}

void Player::print_status() const {
    std::cout << this->get_Colour() << " has " << victory_points
              << " building points, ";
    for (int i = 0; i < 5; i++) {
        std::cout << resource_count.at(i) << " " << print_resource(i)
                  << ((i == 4) ? "." : ", ");
    }
    std::cout << std::endl;
}

void Player::update_player_by_file(std::istringstream &s) {
    for (int i = 0; i < 5; i++) {
        s >> resource_count[i];
    }
    char input;
    int road_num;

    s >> input;  // read r
    while (s >> road_num) {
        roads.emplace_back(static_cast<size_t>(road_num));
    }
    // if break out of loop we have read in h
    s.clear();
    s.ignore();

    int building_num;
    char building_type_input;
    Building_Type building_type;
    while (s >> building_num) {
        s >> building_type_input;
        if (building_type_input == 'B') {
            building_type = Building_Type::Basement;
        } else if (building_type_input == 'H') {
            building_type = Building_Type::House;
        } else if (building_type_input == 'T') {
            building_type = Building_Type::Tower;
        }
        buildings[building_num] = building_type;
    }
    // terminate loop when reach end of file for stringstream
}

void Player::output_status_to_file(std::ofstream &f) const {
    // output resources
    for (auto x : resource_count) {
        f << x << " ";
    }
    // output roads
    f << "r ";
    for (auto x : roads) {
        f << x << " ";
    }
    // output buildings
    f << "h ";
    for (auto kv : buildings) {
        f << kv.first << " " << kv.second << " ";
    }
    f << std::endl;
}

void Player::print_buildings() const {
    std::cout << this->get_Colour() << " has built:" << std::endl;
    for (auto kv : buildings) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }
}

Colour Player::get_Colour() const { return colour; }

void Player::lose_resource_to_geese() {
    int total_resources_count = get_total_resource();

    if (total_resources_count >= 10) {
        int half = total_resources_count / 2;

        std::cout << "Builder " << this->get_Colour() << " loses " << half
                  << " resources to the geese. They lose:" << std::endl;

        // use a time-based seed for the default seed value
        if (!set_seed) {
            seed = std::chrono::system_clock::now().time_since_epoch().count();
        }
        std::mt19937 gen(seed);
        std::vector<int> resource_lost_count{
            0, 0, 0, 0, 0};  // purely used to keep track of cumulative amount
                             // of each resource lost
        std::vector<int> resource_pool;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < this->resource_count[i]; j++)
                resource_pool.push_back(i);

        std::shuffle(resource_pool.begin(), resource_pool.end(), gen);

        for (int i = 0; i < half; i++) {
            resource_lost_count[resource_pool[i]]++;
            this->resource_count[resource_pool[i]]--;
        }

        for (int i = 0; i < 5; i++)
            std::cout << resource_lost_count[i] << " " << print_resource(i)
                      << ((i == 4) ? "." : ", ");
        std::cout << std::endl;
    }
}
// steals a resource from a player assuming they are valid (they have at least
// one resource)
void Player::steal(Player *victim) { victim->robbed(this); }

void Player::robbed(Player *robber) {
    if (!set_seed) {
        seed = std::chrono::system_clock::now().time_since_epoch().count();
    }
    std::mt19937 gen(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist4(
        1, this->get_total_resource());
    int resource = dist4(gen);

    for (int i = 0; i < 5; i++) {
        resource -= this->resource_count[i];
        if (resource <= 0) {
            resource_count[i]--;
            robber->increment_resource(i, 1);
            std::cout << "Builder " << robber->get_Colour() << " steals "
                      << print_resource(i) << " from builder "
                      << this->get_Colour() << "." << std::endl;
            return;
        }
    }
}

std::string Player::print_resource(size_t type) const {
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

int Player::roll_dice() { return dice->generateNumber(); }

void Player::setDice(Dice *new_dice) { dice = new_dice; }

Dice *Player::getDice() const { return dice; }

bool Player::win() const {
    if (victory_points >= 10) {
        return true;
    }
    return false;
}

Player::~Player() {}
