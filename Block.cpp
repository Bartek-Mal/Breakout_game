#include "Block.h"

Block::Block(const sf::Vector2f& size, const sf::Vector2f& position)
{
    rect.setSize(size);
    rect.setPosition(position);
    rect.setFillColor(randomColor()); 
}

void Block::Draw(sf::RenderWindow& window)
{
    window.draw(rect);
}

sf::Color Block::randomColor()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 255);

    return sf::Color(dis(gen), dis(gen), dis(gen));
}
