#ifndef GAME_HPP_
#define GAME_HPP_

#include <SFML/Graphics.hpp>
#include <thread>
#include <atomic>
#include <regex>
#include <filesystem>
#include <vector>
#include <ranges>
#include "Ball.h"
#include "Paddle.h"
#include "BlocksField.h"
#include "GlobalObjects.h"

struct LeaderboardEntry {
    std::string name;
    int score;
    int time;

    bool operator<(const LeaderboardEntry& other) const {
        return score > other.score || (score == other.score && time < other.time);
    }
};

class Game {
private:
    static Ball* ball;
    static Ball* secondBall; 
    static Paddle* paddle;
    static std::atomic<int> score;
    static std::atomic<bool> gameOver;
    static std::atomic<bool> gameOver2;
    static std::atomic<bool> gamePaused;
    static std::atomic<int> timeElapsed;
    static std::thread timerThread;
    static std::string playerName;
    static std::atomic<bool> inputName;
    static std::atomic<bool> startGame;
    static std::atomic<bool> gameStarted;

    static std::vector<LeaderboardEntry> leaderboard;

    static int currentLevel;

    static std::string getLeaderboardFileName();

    static std::string formatTime(int totalSeconds);

public:
    static void displayUniqueBestScores();
    static void initialize();
    static const std::regex nameRegex;
    static void createBall(Ball newBall);
    static void createPaddle(Paddle newPaddle);
    static void addSecondBall(); 
    static bool update(float deltaTime, BlocksField& blocksField);
    static void draw(sf::RenderWindow& window);
    static void reset(BlocksField& blocksField);
    static void reset(MovingBlocksField& blocksField);
    static void reset(RandomBlocksField& blocksField);
    static void resetCurrentLevel(BlocksField& blocksField, MovingBlocksField& movingBlocksField, RandomBlocksField& randomBlocksField);
    static void timerFunction();
    static void stopTimer();
    static void startTimer();
    static int getScore();
    static int getTimeElapsed();
    static void savePlayerName();
    static void saveToLeaderboard();
    static void loadLeaderboard();
    static void drawLeaderboard(sf::RenderWindow& window);
    static bool fileExists(const std::string& fileName);

    static std::string getPlayerName();
    static void setPlayerName(const std::string& name);
    static bool isInputName();
    static void setInputName(bool value);
    static bool isGameOver();
    static void setGameOver(bool value);
    static bool isGameOver2();
    static void setGameOver2(bool value);
    static int getCurrentLevel();
    static void setCurrentLevel(int level);
};

#endif // GAME_HPP_
