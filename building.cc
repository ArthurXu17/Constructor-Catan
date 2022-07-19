#include "player.h"
#include "observer.h"
#include "building.h"

Building::~Building() {
    /*if (owner) {
        delete owner;
    }*/
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
    //do nothing
}
