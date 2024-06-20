#include "BlocksField.h"
#include "Game.h"
import GameAudio;
#include <algorithm> 
#include <random> 

BlocksField::BlocksField(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, int columns, int rows)
{
    sf::Vector2f blockSize(size.x / columns, size.y / rows);

    for (int i = 0; i < columns; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            blocks.push_back(Block(blockSize - sf::Vector2f(4.f, 4.f), position + sf::Vector2f(blockSize.x * i + 2.f, blockSize.y * j + 2.f)));
        }
    }

    std::vector<sf::Vector2f> blockPositions;
    for (auto& block : blocks)
    {
        blockPositions.push_back(block.getPosition());
    }

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(blockPositions.begin(), blockPositions.end(), rng);

    for (int i = 0; i < 3 && i < blockPositions.size(); ++i)
    {
        shrinkBlocks.push_back(blockPositions[i]);
    }
}

void BlocksField::addBallSpawnBlock() {
    if (!blocks.empty()) {
        std::random_device rd;
        std::default_random_engine rng(rd());
        std::uniform_int_distribution<> dist(0, blocks.size() - 1);
        auto it = blocks.begin();
        std::advance(it, dist(rng));
        it->setFillColor(sf::Color::Magenta);
    }
}

int BlocksField::Update(Ball& ball, Paddle& paddle) {
    int initialSize = blocks.size();
    blocks.remove_if([&ball, &paddle, this](const Block& block) {
        bool collided = ball.checkColission(block);
        if (collided) {
            GameAudio::playCollisionSound();

            if (block.getFillColor() == sf::Color::Magenta) {
                Game::addSecondBall();
            }
            auto it = std::find(shrinkBlocks.begin(), shrinkBlocks.end(), block.getPosition());
            if (it != shrinkBlocks.end()) {
                if (!isPaddleShrunk) {
                    paddle.shrink();
                    GameAudio::playShrinkSound(); 
                    isPaddleShrunk = true;
                    shrinkClock.restart();
                }
                shrinkBlocks.erase(it);
            }
        }
        return collided;
        });

    if (isPaddleShrunk && shrinkClock.getElapsedTime().asSeconds() > shrinkDuration) {
        resetShrinkEffect(paddle);
    }

    int currentSize = blocks.size();
    return initialSize - currentSize; 
}

void BlocksField::Draw(sf::RenderWindow& window)
{
    for (auto& block : blocks)
    {
        block.Draw(window);
    }
}

bool BlocksField::allBlocksDestroyed() const
{
    return blocks.empty();
}

void BlocksField::resetShrinkEffect(Paddle& paddle)
{
    paddle.resetSize();
    GameAudio::playResetSizeSound();
    isPaddleShrunk = false;
}


MovingBlocksField::MovingBlocksField(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, int columns, int rows, float speed)
    : BlocksField(size, position, color, columns, rows), speed(speed)
{
    blocks.clear();

    sf::Vector2f blockSize(size.x / 9, size.y / rows); 
    sf::Vector2f initialPosition = position;

    for (int j = 0; j < rows; ++j)
    {
        for (int i = 0; i < columns; ++i)
        {
            Block block(blockSize - sf::Vector2f(4.f, 4.f), initialPosition + sf::Vector2f(blockSize.x * (i * 2 + 2.f), blockSize.y * j + 2.f));
            blocks.push_back(block);
        }
    }

    for (int j = 0; j < rows; ++j)
    {
        if (j % 2 == 0)
        {
            rowDirections.push_back(sf::Vector2f(1.f, 0.f)); 
        }
        else
        {
            rowDirections.push_back(sf::Vector2f(-1.f, 0.f));
        }
    }
}

void MovingBlocksField::moveBlocks(float deltaTime)
{
    int rowIndex = 0;
    for (auto& block : blocks)
    {
        sf::Vector2f pos = block.getPosition();
        pos += rowDirections[rowIndex] * speed * deltaTime;

        if (pos.x <= 0.f || pos.x + block.getSize().x >= GlobalObjects::windowWidth)
        {
            rowDirections[rowIndex].x = -rowDirections[rowIndex].x; 
        }

        block.setPosition(pos);

        if (++rowIndex % 4 == 0)
        {
            rowIndex = 0;
        }
    }
}

RandomBlocksField::RandomBlocksField(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, int numBreakableBlocks)
    : BlocksField(size, position, color, 0, 0), timeSinceLastToggle(0), areUnbreakableBlocksVisible(true)
{
    srand(static_cast<unsigned>(time(0)));

    int maxBlocksX = static_cast<int>(size.x) / 40;
    int maxBlocksY = static_cast<int>(size.y) / 20;
    int maxBlocks = maxBlocksX * maxBlocksY;

    float adjustedBlockWidth = size.x / maxBlocksX;
    float adjustedBlockHeight = 20.f;

    std::vector<sf::Vector2f> possiblePositions;
    for (int i = 0; i < maxBlocksX; ++i) {
        for (int j = 0; j < maxBlocksY; ++j) {
            possiblePositions.emplace_back(i * adjustedBlockWidth, j * adjustedBlockHeight);
        }
    }

    
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(possiblePositions.begin(), possiblePositions.end(), rng);

    
    for (int i = 0; i < numBreakableBlocks; ++i) {
        blocks.push_back(Block(sf::Vector2f(adjustedBlockWidth - 4.f, adjustedBlockHeight - 4.f), position + possiblePositions[i]));
        shrinkBlocks.push_back(possiblePositions[i]);
    }

    for (int i = 0; i < maxBlocksX; ++i) {
        for (int j = 0; j < maxBlocksY; ++j) {
            if (std::find(shrinkBlocks.begin(), shrinkBlocks.end(), sf::Vector2f(i * adjustedBlockWidth, j * adjustedBlockHeight)) == shrinkBlocks.end()) {
                unbreakableBlocks.push_back(Block(sf::Vector2f(adjustedBlockWidth - 4.f, adjustedBlockHeight - 4.f), position + sf::Vector2f(i * adjustedBlockWidth, j * adjustedBlockHeight)));
            }
        }
    }
}

void RandomBlocksField::moveBlocks(float deltaTime)
{
    timeSinceLastToggle += deltaTime;

    if (timeSinceLastToggle >= 4.f)
    {
        toggleUnbreakableBlocks();
        timeSinceLastToggle = 0.f;
    }
}

void RandomBlocksField::Draw(sf::RenderWindow& window)
{
    BlocksField::Draw(window);

    if (areUnbreakableBlocksVisible)
    {
        for (auto& block : unbreakableBlocks)
        {
            block.Draw(window);
        }
    }
}

void RandomBlocksField::toggleUnbreakableBlocks()
{
    areUnbreakableBlocksVisible = !areUnbreakableBlocksVisible;
}