#include <iostream>
#include "grid.h"
#include "player.h"
#include "building.h"

int main(int argc, char** argv) {
    Grid *g = new Grid();
    g->print_grid();
    delete g;
}