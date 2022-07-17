#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
// for enum Resource Class
#include "observer.h"


class Player {
    int victory_points;
    std::vector<int> resource_count;

    public:
        void get_points() const;
        void increment_points();
        void increment_resource(int index, int amount);
        void steal(Player *victim);
        void trade_resources(Player *other, Resource resource_to_give, Resource resource_to_gain);
        ~Player() = default;
};

#endif
