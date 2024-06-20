#ifndef MENU_HPP_
#define MENU_HPP_

#include <SFML/Graphics.hpp>

class Menu
{
protected:
    sf::Font font;
    sf::Text startText;
    sf::Text infoText;
    sf::Text quitText;
    sf::Text infoContent;
    sf::Text backText;
    sf::Text level1Text; 
    sf::Text level2Text; 
    sf::Text level3Text; 
    sf::RectangleShape startButton;
    sf::RectangleShape infoButton;
    sf::RectangleShape quitButton;
    sf::RectangleShape backButton;
    sf::RectangleShape level1Button; 
    sf::RectangleShape level2Button; 
    sf::RectangleShape level3Button; 
    bool showInfo;
    bool showLevels; 

public:
    Menu();
    virtual void handleEvent(const sf::Event& event, bool& startLevel1, bool& startLevel2, bool& startLevel3, bool& quitGame);
    virtual void draw(sf::RenderWindow& window);
};

#endif // MENU_HPP_