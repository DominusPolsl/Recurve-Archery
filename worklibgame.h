#pragma once
#include <SFML/Graphics.hpp>
#include "Target.h"
#include "Wind.h"
#include "Menu.h"
using namespace sf;

void aimMovement(RectangleShape&, Vector2f&, Vector2i&, float&, float, float, float);
