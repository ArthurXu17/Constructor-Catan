#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_
#include <iostream>

enum class Resource {Brick, Energy, Glass, Heat, Wifi, Park};

enum class Building_Type{NoBuilding, Basement, House, Tower};

enum class Colour{NoColour, Blue, Red, Orange, Yellow};

std::ostream &operator<<(std::ostream &out, const Resource r);

std::ostream &operator<<(std::ostream &out, const Building_Type b);

std::ostream &operator<<(std::ostream &out, const Colour c);

#endif