#include <vector>

class Player {
    int victory_points;
    std::vector<int> resource_count;

    public:
        void get_points();
        void increment_points();
        void increment_resource(int index, int amount);
};