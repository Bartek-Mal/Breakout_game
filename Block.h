#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <cmath>
#define M_PI 3.14159265358979323846

#include <SFML/Graphics.hpp>
#include <random>

class Block
{
private:
    sf::RectangleShape rect;
    static sf::Color randomColor(); 

public:
    Block(const sf::Vector2f& size, const sf::Vector2f& position);

    void Draw(sf::RenderWindow& window);

    float left()   const { return rect.getPosition().x; }
    float right()  const { return rect.getPosition().x + rect.getSize().x; }
    float top()    const { return rect.getPosition().y; }
    float bottom() const { return rect.getPosition().y + rect.getSize().y; }

    sf::Vector2f getPosition() const { return rect.getPosition(); }
    sf::Vector2f getSize() const { return rect.getSize(); }
    void setPosition(const sf::Vector2f& position) { rect.setPosition(position); }
    sf::Color getFillColor() const { return rect.getFillColor(); } 
    void setFillColor(const sf::Color& color) { rect.setFillColor(color); } 
};

#endif // BLOCK_HPP_
