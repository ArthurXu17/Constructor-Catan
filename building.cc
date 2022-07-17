#include "player.h"
#include "observer.h"
#include "building.h"

Building::~Building() {
    /*if (owner) {
        delete owner;
    }*/
}

void Building::notify(Subject &s) {
    //do nothing
}
