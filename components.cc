#include "components.h"

std::ostream &operator<<(std::ostream &out, const Resource r) {    
    if (r == Resource::Brick) return out << "BRICK";
    if (r == Resource::Energy) return out << "ENERGY";
    if (r == Resource::Glass) return out << "GLASS";
    if (r == Resource::Heat) return out << "HEAT";
    if (r == Resource::Wifi) return out << "WIFI";
    return out << "PARK";
}

std::ostream &operator<<(std::ostream &out, const Building_Type b) {
    if (b == Building_Type::Basement) return out << "B";
    if (b == Building_Type::House) return out << "H";
    if (b == Building_Type::Tower) return out << "T";
    return out << "NO BUILDING";
}

std::ostream &operator<<(std::ostream &out, const Colour c) {
    if (c == Colour::Blue) return out << "BLUE";
    if (c == Colour::Red) return out << "RED";
    if (c == Colour::Orange) return out << "ORANGE";
    if (c == Colour::Yellow) return out << "YELLOW";
    return out << "NO COLOUR";
}
