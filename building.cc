#include "player.h"
#include "observer.h"
#include "building.h"

Building::Building(Player *owner): owner{owner} {}

Building::~Building() {
    // don't delete the owner, this is handled in main
}

Player* Building::get_Owner() const {
    return owner;
}

Building_Type Building::get_type() const {
    if (resource_gain == 1) {
        return Building_Type::Basement;
    } else if (resource_gain == 2) {
        return Building_Type::House;
    } else if (resource_gain == 3) {
        return Building_Type::Tower;
    } else {
        return Building_Type::NoBuilding;
    }
}

void Building::notify(Subject &s) {
    //increment owner's resource based on the subject's (tile) resource and this's resource_gain amount
    int index = static_cast<int>(s.get_resource());
    owner->increment_resource(index, resource_gain);
}

void Building::upgrade() {
    resource_gain++;
    owner->increment_points();
}