#include <iostream>
#include <string>
#include <cstdlib>

class Dice {
    public:
        virtual int generateNumber() = 0;
};

class RandomDice : public Dice {
    public:
        int generateNumber() override {
            int rolls[] = {2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 11, 11, 12};
            int length = 36;
            int roll = rolls[rand() % length];
            return roll;
        };
};

class LoadedDice : public Dice {
    public:
        int generateNumber() override {
            int min = 2, max = 12;
            int roll = 0;
            while (true) {
                std::cout << "Input a roll between " << min << " and " << max << ": ";
                std::cin >> roll;
                if (roll < min || roll > max)
                    std::cout << "Invalid roll." << std::endl;
                else break;
            }
            return roll;
        }
};


/*int main() {
    Dice *fair = new RandomDice();
    Dice *load = new LoadedDice(); 
    Dice *dice_choice = nullptr;
    // Dice *loaded = new LoadedDice();
    std::string cmd;
    std::string dice;
    while (true) {
    std::cin >> cmd;
    if (cmd == "fair") {
        dice_choice = fair;
    } else if (cmd == "load") {
        dice_choice = load;
    } else if (cmd == "play") {
        dice_choice->generateNumber();
    }
    }

    dice_choice = nullptr;
    delete fair;
    delete load;
    //here's a commment to test git uwu
}*/