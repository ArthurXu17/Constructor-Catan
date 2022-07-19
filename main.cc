#include <iostream>
#include <fstream>
#include <string>
#include "grid.h"
#include "player.h"
#include "building.h"

int main(int argc, char** argv) {
    std::string file_name = "";
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-load") {
            i++;
            file_name = argv[i];
        }
    }
    std::ifstream infile{file_name};
    Grid *g;
    if (file_name == "") {
        g = new Grid();
    } else {
        g = new Grid(infile);
    }
    g->print_grid();

    Player *blue = new Player(Colour::Blue);
    Player *red = new Player(Colour::Red);
    Player *orange = new Player(Colour::Orange);
    Player *yellow = new Player(Colour::Yellow);

    g->test_map();
    /*g->build_building(red, 9);
    g->print_grid();*/

    // for (size_t i = 1; i < 50; i += 5) {
    //     g->build_road(Colour::Yellow, i);
    //     g->print_grid();
    // }
    delete blue;
    delete red;
    delete orange;
    delete yellow;
    delete g;
}