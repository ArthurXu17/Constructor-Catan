#include "dice.h"

Dice::~Dice() {}

RandomDice::RandomDice(bool set_seed_input, unsigned seed_input): 
    set_seed{set_seed_input}, seed{seed_input} 
    {
        // use a time-based seed for the default seed value
        if (!set_seed) {
            seed = std::chrono::system_clock::now().time_since_epoch().count();
        }
        std::mt19937 gen(seed);
    }

int RandomDice::generateNumber() {
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);
    int dice1 = dist6(gen);
    int dice2 = dist6(gen);
    return dice1 + dice2;
}

RandomDice::~RandomDice() {}

int LoadedDice::generateNumber() {
    int min = 2, max = 12;
    int roll = 0;
    while (true) {
        std::cout << "Input a roll between " << min << " and " << max << ": "; // prompt for roll
        std::cin >> roll;
        if (roll < min || roll > max) { // not within range
            std::cout << "Invalid roll." << std::endl;
        }
        else break;
    }
    return roll;
}

LoadedDice::~LoadedDice() {}
