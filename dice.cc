#include "dice.h"

Dice::~Dice() {}

RandomDice::RandomDice(bool set_seed_input, unsigned seed_input): 
    set_seed{set_seed_input}, seed{seed_input} {}

int RandomDice::generateNumber() {
    std::vector<int> rolls = {2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 11, 11, 12};
    // use a time-based seed for the default seed value
    if (!set_seed) {
        seed = std::chrono::system_clock::now().time_since_epoch().count();
    }
    std::default_random_engine rng{seed};
    std::shuffle(rolls.begin(), rolls.end(), rng);
    return rolls[0];
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
