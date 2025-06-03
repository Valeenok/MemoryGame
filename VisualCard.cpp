#include "VisualCard.h"

sf::Font VisualCard::font;

VisualCard::VisualCard(int id, float x, float y, float size)
    : Card(id)
{
    shape.setSize({ size, size });
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Blue);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);

    text.setFont(font);
    text.setString(std::to_string(id));
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Black);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2, bounds.height / 2);
    text.setPosition(x + size / 2, y + size / 2);
}

void VisualCard::draw(sf::RenderWindow& window) {
    window.draw(shape);
    if (revealed() || matched()) {
        window.draw(text);
    }
}

bool VisualCard::contains(sf::Vector2f point) {
    return shape.getGlobalBounds().contains(point);
}

void VisualCard::updateVisualState() {
    if (matched())
        shape.setFillColor(sf::Color::Green);
    else if (revealed())
        shape.setFillColor(sf::Color::White);
    else
        shape.setFillColor(sf::Color::Blue);
}

void VisualCard::loadFont(const std::string& path) {
    font.loadFromFile(path);
}
