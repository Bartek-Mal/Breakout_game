import GameAudio;

#include <SFML/Graphics.hpp>
#include <iostream>
#include "GlobalObjects.h"
#include "BlocksField.h"
#include "Paddle.h"
#include "Game.h"
#include "Menu.h"
#include "PauseMenu.h"

int main() {
    GameAudio::initialize(); 
    GameAudio::playMenuMusic(); 

    if (!Game::fileExists("leaderboards.txt")) {
        std::cerr << "Leaderboard file not found!" << std::endl;
    }

    if (!Game::fileExists("leaderboards2.txt")) {
        std::cerr << "Leaderboard file not found!" << std::endl;
    }

    if (!Game::fileExists("leaderboards3.txt")) {
        std::cerr << "Leaderboard file not found!" << std::endl;
    }

    sf::Event event;
    sf::Clock clock;
    float deltaTime;

    Menu menu;
    PauseMenu pauseMenu;
    bool startLevel1 = false;
    bool startLevel2 = false;
    bool startLevel3 = false;
    bool quitGame = false;
    bool pauseGame = false;
    bool resumeGame = false;
    bool restartGame = false;
    bool mainMenu = false;

    BlocksField blocksField(sf::Vector2f(GlobalObjects::windowWidth, 200.f), sf::Vector2f(0.f, 0.f), sf::Color::Yellow, 9, 7);
    MovingBlocksField movingBlocksField(sf::Vector2f(GlobalObjects::windowWidth, 200.f), sf::Vector2f(0.f, 0.f), sf::Color::Yellow, 4, 7, 150.f);
    RandomBlocksField randomBlocksField(sf::Vector2f(GlobalObjects::windowWidth, 200.f), sf::Vector2f(0.f, 0.f), sf::Color::Yellow, 15);
    Game::initialize();
    Game::createBall(Ball(10.f, sf::Vector2f(225.0710f, 400.f), sf::Color::Red, 300.f, 110.f));
    Game::createPaddle(Paddle(sf::Vector2f(100.f, 10.f), sf::Vector2f(256.f, 450.f), sf::Color::Green, 200.f));

    while (GlobalObjects::window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        while (GlobalObjects::window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                GlobalObjects::window.close();

            if (!startLevel1 && !startLevel2 && !startLevel3) {
                menu.handleEvent(event, startLevel1, startLevel2, startLevel3, quitGame);
                if (quitGame) {
                    GlobalObjects::window.close();
                }
                if (startLevel1 || startLevel2 || startLevel3) {
                    GameAudio::playGameMusic(); 
                }
            }
            else if (pauseGame) {
                pauseMenu.handleEvent(event, resumeGame, restartGame, mainMenu);
                if (resumeGame) {
                    pauseGame = false;
                    Game::startTimer();
                    resumeGame = false;
                }
                if (restartGame) {
                    Game::resetCurrentLevel(blocksField, movingBlocksField, randomBlocksField);
                    pauseGame = false;
                    restartGame = false;
                }
                if (mainMenu) {
                    Game::resetCurrentLevel(blocksField, movingBlocksField, randomBlocksField);
                    startLevel1 = false;
                    startLevel2 = false;
                    startLevel3 = false;
                    pauseGame = false;
                    mainMenu = false;
                    GameAudio::playMenuMusic(); 
                }
            }
            else if (Game::isInputName()) {
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == '\b') {
                        if (!Game::getPlayerName().empty()) {
                            std::string name = Game::getPlayerName();
                            name.pop_back();
                            Game::setPlayerName(name);
                        }
                    }
                    else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                        if (std::regex_match(Game::getPlayerName(), Game::nameRegex)) {
                            Game::setInputName(false);
                            Game::savePlayerName();
                            Game::saveToLeaderboard();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        std::string name = Game::getPlayerName();
                        name += static_cast<char>(event.text.unicode);
                        Game::setPlayerName(name);
                    }
                }
            }
            else if (Game::isGameOver() || Game::isGameOver2()) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    sf::FloatRect mainMenuButtonRect(
                        (GlobalObjects::windowWidth - 200) / 2,
                        GlobalObjects::windowHeight - 100,
                        200,
                        50
                    );

                    if (mainMenuButtonRect.contains(mousePos)) {
                        mainMenu = true;
                        Game::resetCurrentLevel(blocksField, movingBlocksField, randomBlocksField);
                        startLevel1 = false;
                        startLevel2 = false;
                        startLevel3 = false;
                        GameAudio::playMenuMusic(); 
                    }
                }
            }
            else {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    pauseGame = true;
                    Game::stopTimer();
                }
            }
        }

        GlobalObjects::window.clear(sf::Color::Black);

        if (!startLevel1 && !startLevel2 && !startLevel3) {
            menu.draw(GlobalObjects::window);
        }
        else if (pauseGame) {
            pauseMenu.draw(GlobalObjects::window);
        }
        else {
            if (!Game::isGameOver()) {
                if (startLevel1) {
                    Game::update(deltaTime, blocksField);
                    Game::setCurrentLevel(1);
                }
                else if (startLevel2) {
                    movingBlocksField.moveBlocks(deltaTime);
                    Game::update(deltaTime, movingBlocksField);
                    Game::setCurrentLevel(2);
                }
                else if (startLevel3) {
                    randomBlocksField.moveBlocks(deltaTime);
                    Game::update(deltaTime, randomBlocksField);
                    Game::setCurrentLevel(3);
                }
            }

            if (startLevel1) {
                blocksField.Draw(GlobalObjects::window);
            }
            else if (startLevel2) {
                movingBlocksField.Draw(GlobalObjects::window);
            }
            else if (startLevel3) {
                randomBlocksField.Draw(GlobalObjects::window);
            }

            Game::draw(GlobalObjects::window);
        }

        GlobalObjects::window.display();
    }

    return 0;
}
