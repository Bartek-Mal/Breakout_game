#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <iomanip>

namespace fs = std::filesystem;

Ball* Game::ball = nullptr;
Ball* Game::secondBall = nullptr; 
Paddle* Game::paddle = nullptr;
std::atomic<int> Game::score(0);
std::atomic<bool> Game::gameOver(false);
std::atomic<bool> Game::gameOver2(false);
std::atomic<bool> Game::gamePaused(false);
std::atomic<int> Game::timeElapsed(0);
std::atomic<bool> Game::startGame(false);
std::atomic<bool> Game::gameStarted(false);
std::thread Game::timerThread;
std::string Game::playerName = "";
std::atomic<bool> Game::inputName(false);
const std::regex Game::nameRegex("^[A-Za-z0-9_]{3,12}$");
std::vector<LeaderboardEntry> Game::leaderboard;

int Game::currentLevel = 1;

void Game::initialize() {
    score = 0;
    gameOver = false;
    gamePaused = false;
    timeElapsed = 0;
    inputName = false;
    currentLevel = 1;
    startGame = false;
    gameStarted = false;
    startTimer();
}

std::string Game::formatTime(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;

    return oss.str();
}

void Game::createBall(Ball newBall) {
    if (ball != nullptr) {
        delete ball;
    }
    ball = new Ball(newBall);
}

void Game::addSecondBall() {
    if (secondBall == nullptr) {
        secondBall = new Ball(*ball);
        secondBall->setPosition(sf::Vector2f(paddle->getPosition().x + paddle->getSize().x / 2.f - secondBall->getRadius(), paddle->getPosition().y - secondBall->getRadius() * 2.f));
    }
}

void Game::createPaddle(Paddle newPaddle) {
    if (paddle != nullptr) {
        delete paddle;
    }
    paddle = new Paddle(newPaddle);
}

bool Game::update(float deltaTime, BlocksField& blocksField) {
    if (gamePaused) return false;

    if (paddle != nullptr)
        paddle->Update(deltaTime);

    bool ballExists = (ball != nullptr && ball->exist());
    bool secondBallExists = (secondBall != nullptr && secondBall->exist());

    if (ballExists || secondBallExists) {
        if (!gameStarted) {
            startGame = true;
            gameStarted = true;
        }

        if (ballExists) {
            ball->Update(deltaTime);
            ball->checkColission(*paddle);
            int blocksDestroyed = blocksField.Update(*ball, *paddle);
            score += blocksDestroyed;
        }

        if (secondBallExists) {
            secondBall->Update(deltaTime);
            secondBall->checkColission(*paddle);
            int blocksDestroyed = blocksField.Update(*secondBall, *paddle);
            score += blocksDestroyed;
        }

        if (blocksField.allBlocksDestroyed()) {
            gameOver = true;
            inputName = true;
        }

        if (!ballExists && !secondBallExists) {
            gameOver2 = true;
            stopTimer();
            inputName = true;
            return false;
        }

        return true;
    }
    else {
        if (ball != nullptr) {
            delete ball;
            ball = nullptr;
        }
        if (secondBall != nullptr && secondBall->isOutOfBounds()) {
            delete secondBall;
            secondBall = nullptr;
        }
        if (ball == nullptr && secondBall == nullptr) {
            gameOver2 = true;
            stopTimer();
            inputName = true;
            return false;
        }
    }
    return true;
}
void Game::draw(sf::RenderWindow& window) {
    if (gameOver || gameOver2) {
        sf::Font font;
        if (!font.loadFromFile("silkscreen.ttf")) {
            std::cerr << "Failed to load font." << std::endl;
            return;
        }

        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString(gameOver ? "WELL DONE" : "GAME OVER");
        gameOverText.setCharacterSize(50);
        gameOverText.setFillColor(gameOver ? sf::Color::Blue : sf::Color::Red);
        gameOverText.setStyle(sf::Text::Bold);
        gameOverText.setPosition((GlobalObjects::windowWidth - gameOverText.getLocalBounds().width) / 2,
            (GlobalObjects::windowHeight - gameOverText.getLocalBounds().height) / 2);

        window.draw(gameOverText);

        if (inputName) {
            sf::Text inputText;
            inputText.setFont(font);
            inputText.setString("\nEnter Your Name: \n" + playerName + "_");
            inputText.setCharacterSize(30);
            inputText.setFillColor(sf::Color::White);
            inputText.setPosition((GlobalObjects::windowWidth - inputText.getLocalBounds().width) / 2,
                (GlobalObjects::windowHeight - inputText.getLocalBounds().height) / 2 + 50);

            window.draw(inputText);

            if (!std::regex_match(playerName, nameRegex)) {
                sf::Text errorText;
                errorText.setFont(font);
                errorText.setString("Name must be 3-12 characters long\nand contain only letters, digits,\nand underscores.");
                errorText.setCharacterSize(20);
                errorText.setFillColor(sf::Color::Red);
                errorText.setPosition((GlobalObjects::windowWidth - errorText.getLocalBounds().width) / 2,
                    (GlobalObjects::windowHeight - errorText.getLocalBounds().height) / 2 + 200);

                window.draw(errorText);
            }
        }
        else {
            window.clear();
            drawLeaderboard(window);
        }
    }
    else {
        if (ball != nullptr)
            ball->Draw(window);
        if (secondBall != nullptr && secondBall->exist())
            secondBall->Draw(window);
        if (paddle != nullptr)
            paddle->Draw(window);

        sf::Font font;
        if (!font.loadFromFile("silkscreen.ttf")) {
            std::cerr << "Failed to load font." << std::endl;
            return;
        }

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(
            (GlobalObjects::windowWidth - scoreText.getLocalBounds().width) / 2,
            250);

        sf::Text timerText;
        timerText.setFont(font);
        timerText.setString("Time: " + formatTime(timeElapsed));
        timerText.setCharacterSize(30);
        timerText.setFillColor(sf::Color::White);
        timerText.setPosition(
            (GlobalObjects::windowWidth - timerText.getLocalBounds().width) / 2,
            300);

        window.draw(scoreText);
        window.draw(timerText);
    }
}

void Game::reset(BlocksField& blocksField) {
    score = 0;
    gameOver = false;
    gameOver2 = false;
    timeElapsed = 0;
    playerName = "";
    inputName = false;
    gameStarted = false;
    blocksField = BlocksField(sf::Vector2f(GlobalObjects::windowWidth, 200.f), sf::Vector2f(0.f, 0.f), sf::Color::Yellow, 9, 7);
    blocksField.addBallSpawnBlock();
    Game::createBall(Ball(10.f, sf::Vector2f(225.0710f, 400.f), sf::Color::Red, 300.f, 110.f));
    secondBall = nullptr; 
    Game::createPaddle(Paddle(sf::Vector2f(100.f, 10.f), sf::Vector2f(256.f, 450.f), sf::Color::Green, 200.f));
    startTimer();
}

void Game::reset(MovingBlocksField& blocksField) {
    score = 0;
    gameOver = false;
    gameOver2 = false;
    timeElapsed = 0;
    playerName = "";
    inputName = false;
    gameStarted = false;
    blocksField = MovingBlocksField(sf::Vector2f(GlobalObjects::windowWidth, 200.f), sf::Vector2f(0.f, 0.f), sf::Color::Yellow, 4, 7, 150.f);
    Game::createBall(Ball(10.f, sf::Vector2f(225.0710f, 400.f), sf::Color::Red, 300.f, 110.f));
    secondBall = nullptr; 
    Game::createPaddle(Paddle(sf::Vector2f(100.f, 10.f), sf::Vector2f(256.f, 450.f), sf::Color::Green, 200.f));
    startTimer();
}

void Game::reset(RandomBlocksField& blocksField) {
    score = 0;
    gameOver = false;
    gameOver2 = false;
    timeElapsed = 0;
    playerName = "";
    inputName = false;
    gameStarted = false;
    blocksField = RandomBlocksField(sf::Vector2f(GlobalObjects::windowWidth, 200.f), sf::Vector2f(0.f, 0.f), sf::Color::Yellow, 7);
    Game::createBall(Ball(10.f, sf::Vector2f(225.0710f, 400.f), sf::Color::Red, 300.f, 110.f));
    secondBall = nullptr;
    Game::createPaddle(Paddle(sf::Vector2f(100.f, 10.f), sf::Vector2f(256.f, 450.f), sf::Color::Green, 200.f));
    startTimer();
}

void Game::resetCurrentLevel(BlocksField& blocksField, MovingBlocksField& movingBlocksField, RandomBlocksField& randomBlocksField) {
    switch (currentLevel) {
    case 1:
        reset(blocksField);
        break;
    case 2:
        reset(movingBlocksField);
        break;
    case 3:
        reset(randomBlocksField);
        break;
    default:
        break;
    }
}

void Game::timerFunction() {
    while (!gameOver && !gamePaused) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (startGame && !gamePaused) {
            ++timeElapsed;
        }
    }
}

void Game::stopTimer() {
    gamePaused = true;
    if (timerThread.joinable()) {
        timerThread.join();
    }
}

void Game::startTimer() {
    gamePaused = false;
    if (timerThread.joinable()) {
        timerThread.join();
    }
    timerThread = std::thread(timerFunction);
}

int Game::getScore() {
    return score;
}

int Game::getTimeElapsed() {
    return timeElapsed;
}

void Game::savePlayerName() {
    std::ofstream file("users.txt", std::ios::app);
    if (file.is_open()) {
        file << playerName << std::endl;
        file.close();
    }
    else {
        std::cerr << "Unable to open file users.txt" << std::endl;
    }
}

std::string Game::getLeaderboardFileName() {
    switch (currentLevel) {
    case 1:
        return "leaderboards.txt";
    case 2:
        return "leaderboards2.txt";
    case 3:
        return "leaderboards3.txt";
    default:
        return "leaderboards.txt";
    }
}

void Game::saveToLeaderboard() {
    std::ofstream file(getLeaderboardFileName(), std::ios::app);
    if (file.is_open()) {
        file << playerName << " " << score << " " << timeElapsed << "s" << std::endl;
        file.close();
    }
    else {
        std::cerr << "Unable to open file " << getLeaderboardFileName() << std::endl;
    }
}

void Game::loadLeaderboard() {
    leaderboard.clear();
    std::ifstream file(getLeaderboardFileName());
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        LeaderboardEntry entry;
        if (iss >> entry.name >> entry.score >> entry.time) {
            leaderboard.push_back(entry);
        }
    }
    file.close();

    std::ranges::sort(leaderboard, [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
        if (a.name == b.name) {
            return std::tie(b.score, a.time) < std::tie(a.score, b.time);
        }
        return a.score > b.score;
        });

    leaderboard.erase(std::unique(leaderboard.begin(), leaderboard.end(),
        [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
            return a.name == b.name;
        }), leaderboard.end());
}


void Game::displayUniqueBestScores() {
    for (const auto& entry : leaderboard) {
        std::cout << "Name: " << entry.name << ", Score: " << entry.score << ", Time: " << entry.time << "s\n";
    }
}

void Game::drawLeaderboard(sf::RenderWindow& window) {
    loadLeaderboard();

    sf::Font font;
    if (!font.loadFromFile("silkscreen.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return;
    }

    sf::Text leaderboardTitle;
    leaderboardTitle.setFont(font);
    leaderboardTitle.setString("Leaderboard");
    leaderboardTitle.setCharacterSize(40);
    leaderboardTitle.setFillColor(sf::Color::White);
    leaderboardTitle.setPosition((GlobalObjects::windowWidth - leaderboardTitle.getLocalBounds().width) / 2, 50);

    window.draw(leaderboardTitle);

    int yOffset = 100;
    for (const auto& entry : leaderboard) {
        sf::Text entryText;
        entryText.setFont(font);
        entryText.setString(entry.name + " " + std::to_string(entry.score) + " " + formatTime(entry.time));
        entryText.setCharacterSize(30);
        entryText.setFillColor(sf::Color::White);
        entryText.setPosition(100, yOffset);

        window.draw(entryText);
        yOffset += 40;
    }

    sf::RectangleShape mainMenuButton(sf::Vector2f(200, 50));
    mainMenuButton.setFillColor(sf::Color::Green);
    mainMenuButton.setPosition((GlobalObjects::windowWidth - mainMenuButton.getSize().x) / 2, GlobalObjects::windowHeight - 100);

    sf::Text mainMenuText;
    mainMenuText.setFont(font);
    mainMenuText.setString("Main Menu");
    mainMenuText.setCharacterSize(24);
    mainMenuText.setFillColor(sf::Color::Black);
    mainMenuText.setPosition(
        mainMenuButton.getPosition().x + (mainMenuButton.getSize().x - mainMenuText.getLocalBounds().width) / 2,
        mainMenuButton.getPosition().y + (mainMenuButton.getSize().y - mainMenuText.getLocalBounds().height) / 2 - 5
    );

    window.draw(mainMenuButton);
    window.draw(mainMenuText);
}

bool Game::fileExists(const std::string& fileName) {
    return fs::exists(fileName);
}

std::string Game::getPlayerName() { return playerName; }
void Game::setPlayerName(const std::string& name) { playerName = name; }

bool Game::isInputName() { return inputName; }
void Game::setInputName(bool value) { inputName = value; }

bool Game::isGameOver() { return gameOver; }
void Game::setGameOver(bool value) { gameOver = value; }

bool Game::isGameOver2() { return gameOver2; }
void Game::setGameOver2(bool value) { gameOver2 = value; }

int Game::getCurrentLevel() { return currentLevel; }
void Game::setCurrentLevel(int level) { currentLevel = level; }
