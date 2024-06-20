#ifndef PAUSEMENU_HPP_
#define PAUSEMENU_HPP_

#include "Menu.h"

class PauseMenu : public Menu
{
private:
    sf::Text resumeText;
    sf::Text restartText;
    sf::Text mainMenuText;
    sf::RectangleShape resumeButton;
    sf::RectangleShape restartButton;
    sf::RectangleShape mainMenuButton;

public:
    PauseMenu();
    void handleEvent(const sf::Event& event, bool& resumeGame, bool& restartGame, bool& mainMenu);
    void draw(sf::RenderWindow& window) override;
};

#endif // PAUSEMENU_HPP_
