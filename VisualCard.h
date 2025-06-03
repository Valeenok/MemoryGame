#pragma once
#include <SFML/Graphics.hpp>
#include "Card.h"

class VisualCard : public Card {
private:
    sf::RectangleShape shape;
    sf::Text text;
    static sf::Font font;

public:
    VisualCard(int id, float x, float y, float size);
    void draw(sf::RenderWindow& window);
    bool contains(sf::Vector2f point);
    void updateVisualState();
    static void loadFont(const std::string& path);
};
