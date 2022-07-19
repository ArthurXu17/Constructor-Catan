#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
// for enum Resource Class
#include "observer.h"

enum class Colour{NoColour, Blue, Red, Orange, Yellow};


class Player {
    int victory_points = 0;
    std::vector<int> resource_count = {0, 0, 0, 0, 0};
    Colour colour;

    public:
        Player(Colour colour);
        int get_points() const;
        void increment_points();
        void increment_resource(int index, int amount);
        void steal(Player *victim);
        void trade_resources(Player *other, Resource resource_to_give, Resource resource_to_gain);
        Colour get_Colour() const;
        ~Player() = default;
};

#endif
