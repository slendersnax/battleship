#ifndef COLOURS_H
#define COLOURS_H

#include <SFML/Graphics.hpp>

enum class Colour { White, Black, Grey, Red, Green, Blue, DarkBlue };

sf::Color getSzin(Colour colour);

#endif