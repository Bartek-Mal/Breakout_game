#ifndef PADDLE_HPP_
#define PADDLE_HPP_

#include <SFML/Graphics.hpp>
#include "GlobalObjects.h"

class Paddle
{
private:
    sf::RectangleShape paddle;
    float speed;
    sf::Vector2f originalSize; 

public:
    Paddle(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, float speed);

    void Update(float deltaTime);
    sf::Vector2f getPosition() const { return paddle.getPosition(); }
    sf::Vector2f getSize() const { return paddle.getSize(); }

    void Draw(sf::RenderWindow& window);
    void shrink(); 
    void resetSize();

    float left()   const { return paddle.getPosition().x - paddle.getSize().x / 2.f; }
    float right()  const { return paddle.getPosition().x + paddle.getSize().x / 2.f; }
    float top()    const { return paddle.getPosition().y - paddle.getSize().y / 2.f; }
    float bottom() const { return paddle.getPosition().y + paddle.getSize().y / 2.f; }
};

#endif // PADDLE_HPP_
