#include "player.h"
#include "observer.h"

enum class Building_Type{Basement, House, Tower};

class Building : public Observer {
    Player *owner;
    int resource_gain;

    public:
        void notify(Subject &s);
};