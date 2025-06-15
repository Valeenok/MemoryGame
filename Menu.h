#pragma once
#include <SFML/Graphics.hpp>
#include "Level.h"

class Menu {
private:

    sf::Font font;
    sf::Texture menuBackgroundTexture;
    sf::Sprite menuBackgroundSprite;
    sf::Text titleText;
    sf::RectangleShape easyButton, mediumButton, hardButton;
    sf::Text easyText, mediumText, hardText;
    sf::Vector2u windowSize;

public:
    Menu();

    bool initialize(const sf::Vector2u& windowSize);
    void updateHover(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    Level processClick(float x, float y);

private:
    bool loadAssets();
    void setupBackground();
    void setupTitle();
    void setupButtons();
    void setupButton(sf::RectangleShape& button, sf::Text& text,
        const std::string& label, float x, float y,
        const sf::Vector2f& size, const sf::Color& color,
        float textY);
};