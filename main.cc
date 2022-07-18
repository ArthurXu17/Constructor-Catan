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
    delete g;
}