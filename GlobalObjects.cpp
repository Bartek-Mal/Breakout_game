#include "GlobalObjects.h"

const float GlobalObjects::windowWidth = 512.f;
const float GlobalObjects::windowHeight = 512.f;
sf::RenderWindow GlobalObjects::window(sf::VideoMode(windowWidth, windowHeight), "Arkanoid", sf::Style::Close | sf::Style::Titlebar);