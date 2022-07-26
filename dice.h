#ifndef _DICE_H_
#define _DICE_H_

#include <iostream>
#include <string>
#include <cstdlib>

//for random
#include <chrono>
#include <random>
#include <algorithm>

class Dice {

    public:
        virtual int generateNumber() = 0;
        virtual ~Dice();
};

class RandomDice : public Dice {
    bool set_seed;
    unsigned seed;
    std::mt19937 gen;
    
    public:
        RandomDice(bool set_seed_input, unsigned seed_input, std::mt19937 gen_input);
        int generateNumber() override;
        ~RandomDice();
};

class LoadedDice : public Dice {
    public:
        int generateNumber() override;
        ~LoadedDice();
};

#endif
