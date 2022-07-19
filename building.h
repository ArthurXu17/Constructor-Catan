#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "player.h"
#include "observer.h"

enum class Building_Type{NoBuilding, Basement, House, Tower};

class Building : public Observer {
    Player *owner;
    int resource_gain;

    public:
        void notify(Subject &s);
        ~Building();
        Player *get_Owner() const;
        Building_Type get_type() const;
};

#endif
