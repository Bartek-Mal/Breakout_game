#include "Paddle.h"

Paddle::Paddle(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, float speed)
{
    paddle.setSize(size);
    paddle.setPosition(position);
    paddle.setFillColor(color);
    paddle.setOrigin(size.x / 2.f, size.y / 2.f);

    this->speed = speed;
    this->originalSize = size; 
}

void Paddle::Update(float deltaTime)
{
    float moveDistance = speed * deltaTime;
    float halfPaddleWidth = paddle.getSize().x / 2.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && (paddle.getPosition().x - halfPaddleWidth > 0.f))
    {
        paddle.move(-moveDistance, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (paddle.getPosition().x + halfPaddleWidth < GlobalObjects::windowWidth))
    {
        paddle.move(moveDistance, 0.f);
    }
}

void Paddle::Draw(sf::RenderWindow& window)
{
    window.draw(paddle);
}

void Paddle::shrink()
{
    paddle.setSize(sf::Vector2f(originalSize.x / 2.f, originalSize.y));
    paddle.setOrigin(paddle.getSize().x / 2.f, paddle.getSize().y / 2.f);
}

void Paddle::resetSize()
{
    paddle.setSize(originalSize);
    paddle.setOrigin(paddle.getSize().x / 2.f, paddle.getSize().y / 2.f);
}
