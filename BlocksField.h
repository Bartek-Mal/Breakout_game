#ifndef BLOCKSFIELD_HPP_
#define BLOCKSFIELD_HPP_

#include <SFML/Graphics.hpp>
#include <list>
#include "Block.h"
#include "Ball.h"
#include <vector>
#include <cstdlib>
#include <ctime>

class BlocksField
{
protected:
    std::list<Block> blocks;
    std::vector<sf::Vector2f> shrinkBlocks; 
    bool isPaddleShrunk = false; 
    float shrinkDuration = 8.0f; 
    sf::Clock shrinkClock; 

public:
    BlocksField(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, int columns, int rows);
    virtual int Update(Ball& ball, Paddle& paddle); 
    virtual void Draw(sf::RenderWindow& window);
    virtual bool allBlocksDestroyed() const; 
    virtual void moveBlocks(float deltaTime) {}; 
    void resetShrinkEffect(Paddle& paddle); 
    void addBallSpawnBlock(); 
};

class MovingBlocksField : public BlocksField
{
private:
    sf::Vector2f direction;
    float speed;
    std::vector<sf::Vector2f> rowDirections;

public:
    MovingBlocksField(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, int columns, int rows, float speed);
    void moveBlocks(float deltaTime) override;
};

class RandomBlocksField : public BlocksField
{
private:
    std::vector<Block> unbreakableBlocks;
    std::vector<sf::Vector2f> unbreakableBlockPositions;
    float timeSinceLastToggle;
    bool areUnbreakableBlocksVisible;

public:
    RandomBlocksField(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, int numBreakableBlocks);
    void moveBlocks(float deltaTime) override;
    void Draw(sf::RenderWindow& window) override;
    void toggleUnbreakableBlocks();
};

#endif // BLOCKSFIELD_HPP_
