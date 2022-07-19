#include "player.h"

Player::Player(Colour colour): colour{colour} {}

int Player::get_points() const {
    return victory_points;
}

Colour Player::get_Colour() const {
    return colour;
}