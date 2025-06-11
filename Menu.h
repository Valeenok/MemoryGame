#pragma once
#include <SFML/Graphics.hpp>
#include "Level.h"

class Menu {
private:
    // Шрифт
    sf::Font font;

    // Фон меню
    sf::Texture menuBackgroundTexture;
    sf::Sprite menuBackgroundSprite;

    // Заголовок
    sf::Text titleText;

    // Кнопки та їх тексти
    sf::RectangleShape easyButton, mediumButton, hardButton;
    sf::Text easyText, mediumText, hardText;

    // Розміри вікна (для правильного позиціювання)
    sf::Vector2u windowSize;

public:
    Menu();

    // Ініціалізація
    bool initialize(const sf::Vector2u& windowSize);

    // Оновлення та відрисовка
    void updateHover(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    Level processClick(float x, float y);

private:
    // Допоміжні методи
    bool loadAssets();
    void setupBackground();
    void setupTitle();
    void setupButtons();
    void setupButton(sf::RectangleShape& button, sf::Text& text,
        const std::string& label, float x, float y,
        const sf::Vector2f& size, const sf::Color& color,
        float textY);
};