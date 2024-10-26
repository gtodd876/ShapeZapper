//
// Created by George Matthews on 10/26/24.
//

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include "Vec2.hpp"

class Component
{
public:
    bool exists{false};
};

class CTransform : public Component
{
public:
    Vec2f pos = {0.0, 0.0};
    Vec2f velocity = {0.0, 0.0};
    float angle = 0.0;

    CTransform() = default;
    CTransform(const Vec2f &p, const Vec2f &v, float a) : pos(p), velocity(v), angle(a) {}
};

class CShape : public Component
{
public:
    sf::CircleShape circle;

    CShape() = default;
    CShape(const float radius, const size_t points, const sf::Color &fill, const sf::Color &outline,
           const float thickness) : circle(radius, points) {
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius);
    }
};

class CCollision : public Component
{
public:
    float radius{0};

    CCollision() = default;
    explicit CCollision(const float r) : radius(r) {}
};

class CScore : public Component
{
public:
    int score = 0;

    CScore() = default;
    explicit CScore(const int s) : score(s) {}
};

class CLifeSpan : public Component
{
public:
    int lifespan{0};
    int remaining{0};
    CLifeSpan() = default;
    explicit CLifeSpan(const int totalLifeSpan) : lifespan(totalLifeSpan), remaining(totalLifeSpan) {}
};

class CInput : public Component
{
public:
    bool up{false};
    bool down{false};
    bool left{false};
    bool right{false};
    bool shoot{false};

    CInput() = default;
};

// TODO: ADD SPECIAL WEAPON CLASS

#endif // COMPONENTS_HPP
