#include "tile.h"

Tile::Tile(Resource r, int val, bool goose): resource{r}, dice_value{val}, hasGoose{goose} {}

Resource Tile::get_resource() const{
    return resource;
}

int Tile::get_dice() const {
    return dice_value;
}

bool Tile::getGooseStatus() const {
    return hasGoose;
}

Tile::~Tile() {}