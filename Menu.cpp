#include "Menu.h"
#include "GlobalObjects.h"
#include <iostream>

Menu::Menu() : showInfo(false), showLevels(false)
{
    if (!font.loadFromFile("silkscreen.ttf"))
    {
        std::cerr << "Failed to load font." << std::endl;
    }

    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::Black);

    infoText.setFont(font);
    infoText.setString("Info");
    infoText.setCharacterSize(24);
    infoText.setFillColor(sf::Color::Black);

    quitText.setFont(font);
    quitText.setString("Quit Game");
    quitText.setCharacterSize(24);
    quitText.setFillColor(sf::Color::Black);

    level1Text.setFont(font);
    level1Text.setString("Level 1");
    level1Text.setCharacterSize(24);
    level1Text.setFillColor(sf::Color::Black);

    level2Text.setFont(font);
    level2Text.setString("Level 2");
    level2Text.setCharacterSize(24);
    level2Text.setFillColor(sf::Color::Black);

    level3Text.setFont(font);
    level3Text.setString("Level 3");
    level3Text.setCharacterSize(24);
    level3Text.setFillColor(sf::Color::Black);

    infoContent.setFont(font);
    infoContent.setString("This is a simple Arkanoid game.\nUse the left and right arrow\nkeys to move the paddle\nand prevent the ball\nfrom falling.\nDestroy all the blocks to win.");
    infoContent.setCharacterSize(24);
    infoContent.setFillColor(sf::Color::White);
    infoContent.setStyle(sf::Text::Bold);

    backText.setFont(font);
    backText.setString("Back");
    backText.setCharacterSize(24);
    backText.setFillColor(sf::Color::Black);

    startButton.setSize(sf::Vector2f(200, 50));
    startButton.setFillColor(sf::Color::Green);

    infoButton.setSize(sf::Vector2f(200, 50));
    infoButton.setFillColor(sf::Color::Blue);

    quitButton.setSize(sf::Vector2f(200, 50));
    quitButton.setFillColor(sf::Color::Red);

    level1Button.setSize(sf::Vector2f(200, 50));
    level1Button.setFillColor(sf::Color::Yellow);

    level2Button.setSize(sf::Vector2f(200, 50));
    level2Button.setFillColor(sf::Color::Cyan);

    level3Button.setSize(sf::Vector2f(200, 50));
    level3Button.setFillColor(sf::Color::Magenta);

    backButton.setSize(sf::Vector2f(200, 50));
    backButton.setFillColor(sf::Color::Yellow);

    // centrowanie przycisków
    float windowWidth = GlobalObjects::windowWidth;
    float windowHeight = GlobalObjects::windowHeight;

    startButton.setPosition((windowWidth - startButton.getSize().x) / 2, 100);
    infoButton.setPosition((windowWidth - infoButton.getSize().x) / 2, 200);
    quitButton.setPosition((windowWidth - quitButton.getSize().x) / 2, 300);

    level1Button.setPosition((windowWidth - level1Button.getSize().x) / 2, 100);
    level2Button.setPosition((windowWidth - level2Button.getSize().x) / 2, 200);
    level3Button.setPosition((windowWidth - level3Button.getSize().x) / 2, 300);

    backButton.setPosition((windowWidth - backButton.getSize().x) / 2, 400);

    // centrowanie tekstu na przyciskach
    startText.setPosition(
        startButton.getPosition().x + (startButton.getSize().x - startText.getLocalBounds().width) / 2,
        startButton.getPosition().y + (startButton.getSize().y - startText.getLocalBounds().height) / 2 - 5
    );
    infoText.setPosition(
        infoButton.getPosition().x + (infoButton.getSize().x - infoText.getLocalBounds().width) / 2,
        infoButton.getPosition().y + (infoButton.getSize().y - infoText.getLocalBounds().height) / 2 - 5
    );
    quitText.setPosition(
        quitButton.getPosition().x + (quitButton.getSize().x - quitText.getLocalBounds().width) / 2,
        quitButton.getPosition().y + (quitButton.getSize().y - quitText.getLocalBounds().height) / 2 - 5
    );
    level1Text.setPosition(
        level1Button.getPosition().x + (level1Button.getSize().x - level1Text.getLocalBounds().width) / 2,
        level1Button.getPosition().y + (level1Button.getSize().y - level1Text.getLocalBounds().height) / 2 - 5
    );
    level2Text.setPosition(
        level2Button.getPosition().x + (level2Button.getSize().x - level2Text.getLocalBounds().width) / 2,
        level2Button.getPosition().y + (level2Button.getSize().y - level2Text.getLocalBounds().height) / 2 - 5
    );
    level3Text.setPosition(
        level3Button.getPosition().x + (level3Button.getSize().x - level3Text.getLocalBounds().width) / 2,
        level3Button.getPosition().y + (level3Button.getSize().y - level3Text.getLocalBounds().height) / 2 - 5
    );
    backText.setPosition(
        backButton.getPosition().x + (backButton.getSize().x - backText.getLocalBounds().width) / 2,
        backButton.getPosition().y + (backButton.getSize().y - backText.getLocalBounds().height) / 2 - 5
    );
}

void Menu::handleEvent(const sf::Event& event, bool& startLevel1, bool& startLevel2, bool& startLevel3, bool& quitGame)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (showInfo)
        {
            if (backButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            {
                showInfo = false;
            }
        }
        else if (showLevels)
        {
            if (level1Button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            {
                startLevel1 = true;
            }
            else if (level2Button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            {
                startLevel2 = true;
            }
            else if (level3Button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            {
                startLevel3 = true;
            }
            else if (backButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            {
                showLevels = false;
            }
        }
        else
        {
            if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            {
                showLevels = true;
            }
            else if (infoButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            {
                showInfo = true;
            }
            else if (quitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            {
                quitGame = true;
            }
        }
    }
}

void Menu::draw(sf::RenderWindow& window)
{
    if (showInfo)
    {
        window.draw(infoContent);
        window.draw(backButton);
        window.draw(backText);
    }
    else if (showLevels)
    {
        window.draw(level1Button);
        window.draw(level1Text);
        window.draw(level2Button);
        window.draw(level2Text);
        window.draw(level3Button);
        window.draw(level3Text);
        window.draw(backButton);
        window.draw(backText);
    }
    else
    {
        window.draw(startButton);
        window.draw(startText);
        window.draw(infoButton);
        window.draw(infoText);
        window.draw(quitButton);
        window.draw(quitText);
    }
}
