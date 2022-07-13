#include <iostream>
#include <string>
class Dice {
    public:
        virtual int generateNumber() = 0;
};

class RandomDice : public Dice {
    public:
        int generateNumber() override;
};

class LoadedDice : public Dice {
    public:
        int generateNumber() override;
};


int main() {
    Dice *fair = new RandomDice();
    Dice *load = new LoadedDice(); 
    Dice *dice_choice = nullptr;
    // Dice *loaded = new LoadedDice();
    string cmd;
    string dice;
    std::cin >> cmd;
    if (cmd == "fair") {
        dice_choice = fair;
    } else if (cmd == "load") {
        dice_choice = load;
    } else if (cmd == "play") {
        dice_choice->generateNumber();
    }

}