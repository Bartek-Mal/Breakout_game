#include "PauseMenu.h"
#include "GlobalObjects.h"
#include "Game.h"
#include <iostream>

PauseMenu::PauseMenu()
{
    if (!font.loadFromFile("silkscreen.ttf"))
    {
        std::cerr << "Failed to load font." << std::endl;
    }

    resumeText.setFont(font);
    resumeText.setString("Resume");
    resumeText.setCharacterSize(24);
    resumeText.setFillColor(sf::Color::Black);

    restartText.setFont(font);
    restartText.setString("Restart");
    restartText.setCharacterSize(24);
    restartText.setFillColor(sf::Color::Black);

    mainMenuText.setFont(font);
    mainMenuText.setString("Main Menu");
    mainMenuText.setCharacterSize(24);
    mainMenuText.setFillColor(sf::Color::Black);

    resumeButton.setSize(sf::Vector2f(200, 50));
    resumeButton.setFillColor(sf::Color::Green);

    restartButton.setSize(sf::Vector2f(200, 50));
    restartButton.setFillColor(sf::Color::Blue);

    mainMenuButton.setSize(sf::Vector2f(200, 50));
    mainMenuButton.setFillColor(sf::Color::Red);

  
    float windowWidth = GlobalObjects::windowWidth;
    float windowHeight = GlobalObjects::windowHeight;

    resumeButton.setPosition((windowWidth - resumeButton.getSize().x) / 2, 100);
    restartButton.setPosition((windowWidth - restartButton.getSize().x) / 2, 200);
    mainMenuButton.setPosition((windowWidth - mainMenuButton.getSize().x) / 2, 300);


    resumeText.setPosition(
        resumeButton.getPosition().x + (resumeButton.getSize().x - resumeText.getLocalBounds().width) / 2,
        resumeButton.getPosition().y + (resumeButton.getSize().y - resumeText.getLocalBounds().height) / 2 - 5
    );
    restartText.setPosition(
        restartButton.getPosition().x + (restartButton.getSize().x - restartText.getLocalBounds().width) / 2,
        restartButton.getPosition().y + (restartButton.getSize().y - restartText.getLocalBounds().height) / 2 - 5
    );
    mainMenuText.setPosition(
        mainMenuButton.getPosition().x + (mainMenuButton.getSize().x - mainMenuText.getLocalBounds().width) / 2,
        mainMenuButton.getPosition().y + (mainMenuButton.getSize().y - mainMenuText.getLocalBounds().height) / 2 - 5
    );
}

void PauseMenu::handleEvent(const sf::Event& event, bool& resumeGame, bool& restartGame, bool& mainMenu)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (resumeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
        {
            resumeGame = true;
        }
        else if (restartButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
        {
            restartGame = true;
        }
        else if (mainMenuButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
        {
            mainMenu = true;
        }
    }
}

void PauseMenu::draw(sf::RenderWindow& window)
{
    window.draw(resumeButton);
    window.draw(resumeText);
    window.draw(restartButton);
    window.draw(restartText);
    window.draw(mainMenuButton);
    window.draw(mainMenuText);
}
