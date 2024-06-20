#ifndef GLOBALOBJECTS_HPP_
#define GLOBALOBJECTS_HPP_

#include <SFML/Graphics.hpp>

class GlobalObjects
{
private:
    GlobalObjects();

public:
    static const float windowWidth, windowHeight;
    static sf::RenderWindow window;
};

#endif // GLOBALOBJECTS_HPP_