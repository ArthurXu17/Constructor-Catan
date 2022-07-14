#include "observer.h"

class Tile : public Subject {
    const Resource resource;
    const int dice_value;
    bool hasGoose;

    public:
        ~Tile();
        Tile(Resource r, int val, bool goose);
        Resource get_resource() const;
        Resource set_resource();
        bool getGooseStatus() const;
};