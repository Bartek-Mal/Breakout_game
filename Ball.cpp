#include "Ball.h"

Ball::Ball(float radius, const sf::Vector2f& position, const sf::Color& color, float speed, float angle) {
    circle.setRadius(radius);
    circle.setPosition(position);
    circle.setFillColor(color);
    circle.setOrigin(radius, radius);

    this->speed = speed;
    setAngle(angle);
}

void Ball::setAngle(float angle) {
    velocity.x = speed * std::cos(angle * M_PI / 180);
    velocity.y = -speed * std::sin(angle * M_PI / 180);
}

float Ball::getAngle() {
    float angle = std::atan2(-velocity.y, velocity.x) * 180 / M_PI;
    if (angle < 0.f)
        angle += 360.f;
    return angle;
}

bool Ball::checkColission(const Block& block) {
    if ((getX() >= block.left() && getX() <= block.right()) ||
        (getY() >= block.top() && getY() <= block.bottom())) {
        if (left() < block.right() && right() > block.left() && top() < block.bottom() && bottom() > block.top()) {
            if (getX() < block.left() || getX() > block.right()) {
                velocity.x *= -1;
            }
            else {
                velocity.y *= -1;
            }
            return true;
        }
    }
    else {
        auto distance = [](sf::Vector2f x1, sf::Vector2f x2)->float {
            return std::sqrt(pow(x1.x - x2.x, 2) + pow(x1.y - x2.y, 2));
            };

        sf::Vector2f corner;

        if (distance(getPosition(), sf::Vector2f(block.left(), block.top())) <= getRadius())
            corner = sf::Vector2f(block.left(), block.top());
        else if (distance(getPosition(), sf::Vector2f(block.left(), block.bottom())) <= getRadius())
            corner = sf::Vector2f(block.left(), block.bottom());
        else if (distance(getPosition(), sf::Vector2f(block.right(), block.top())) <= getRadius())
            corner = sf::Vector2f(block.right(), block.top());
        else if (distance(getPosition(), sf::Vector2f(block.right(), block.bottom())) <= getRadius())
            corner = sf::Vector2f(block.right(), block.bottom());
        else
            return false;

        float a = (-getY() + corner.y) / (getX() - corner.x);
        a = -1 / a;
        float alpha = std::atan(a) * 180 / M_PI;
        if (alpha < 0)
            alpha += 180;
        float beta = getAngle();

        float angle = 2 * alpha - beta;
        if (angle > 180.f)
            angle -= 360.f;
        else if (angle <= -180.f)
            angle += 360.f;

        if (angle <= 0.f && angle > -8.f)
            angle = -8.f;
        else if (angle > 0.f && angle < 8.f)
            angle = 8.f;

        setAngle(angle);

        return true;
    }

    return false;
}

bool Ball::checkColission(const Paddle& paddle) {
    if (left() < paddle.right() && right() > paddle.left() && top() < paddle.bottom() && bottom() >= paddle.top()) {
        float deviation = 50.f;
        bool leftSide = getX() < paddle.getPosition().x;
        float distanceFromCenter = std::abs(getX() - paddle.getPosition().x);
        float percantage = distanceFromCenter / (paddle.getSize().x / 2.f);
        float angle = 90.f - percantage * deviation * (leftSide ? -1.f : 1.f);
        setAngle(angle);

        return true;
    }
    return false;
}

void Ball::Update(float deltaTime) {
    circle.move(velocity * deltaTime);
    if (left() <= 0.f)
        velocity.x = -velocity.x;
    if (right() >= GlobalObjects::windowWidth)
        velocity.x = -velocity.x;
    if (top() <= 0.f)
        velocity.y = -velocity.y;
}

void Ball::Draw(sf::RenderWindow& window) {
    window.draw(circle);
}

bool Ball::isOutOfBounds() const {
    return bottom() > GlobalObjects::windowHeight;
}

void Ball::setPosition(const sf::Vector2f& position) {
    circle.setPosition(position);
}
