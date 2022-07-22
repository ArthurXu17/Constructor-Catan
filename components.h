#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_
#include <iostream>

enum class Resource {Brick, Energy, Glass, Heat, Wifi, Park};

// std::ostream &operator<<(std::ostream &out, const Resource r) {
//     if (r == Resource::Brick) return out << "BRICK";
//     if (r == Resource::Energy) return out << "ENERGY";
//     if (r == Resource::Glass) return out << "GLASS";
//     if (r == Resource::Heat) return out << "HEAT";
//     if (r == Resource::Wifi) return out << "WIFI";
//     return out << "PARK";
// }

enum class Building_Type{NoBuilding, Basement, House, Tower};

// std::ostream &operator<<(std::ostream &out, const Building_Type b) {
//     if (b == Building_Type::Basement) return out << "BASEMENT";
//     if (b == Building_Type::House) return out << "HOUSE";
//     if (b == Building_Type::Tower) return out << "TOWER";
//     return out << "NO BUILDING";
// }

enum class Colour{NoColour, Blue, Red, Orange, Yellow};

// std::ostream &operator<<(std::ostream &out, const Colour c) {
//     if (c == Colour::Blue) return out << "BLUE";
//     if (c == Colour::Red) return out << "RED";
//     if (c == Colour::Orange) return out << "ORANGE";
//     if (c == Colour::Yellow) return out << "YELLOW";
//     return out << "NO COLOUR";
// }

#endif