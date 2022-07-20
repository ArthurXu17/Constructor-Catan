#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "player.h"
#include "observer.h"

class Building : public Observer {
    Player *owner;
    int resource_gain = 1;

    public:
        Building(Player *owner);
        void notify(Subject &s);
        ~Building();
        Player *get_Owner() const override;
        Building_Type get_type() const;
        int get_resource_gain() const override; // added
        void upgrade();
};

#endif
