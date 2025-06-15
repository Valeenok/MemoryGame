#include "Menu.h"
#include <iostream>
#include <optional>

Menu::Menu() {
}

bool Menu::initialize(const sf::Vector2u& windowSize) {
    this->windowSize = windowSize;

    if (!loadAssets()) {
        return false;
    }

    setupBackground();
    setupTitle();
    setupButtons();

    return true;
}

bool Menu::loadAssets() {
    if (!font.loadFromFile("assets/font/arial.ttf")) {
        std::cerr << "Could not load font!\n";
        return false;
    }

    if (!menuBackgroundTexture.loadFromFile("assets/menu_background.jpg")) {
        std::cerr << "Could not load menu background image!\n";
        return false;
    }

    return true;
}

void Menu::setupBackground() {
    menuBackgroundSprite.setTexture(menuBackgroundTexture);
    sf::Vector2u textureSize = menuBackgroundTexture.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    menuBackgroundSprite.setScale(scaleX, scaleY);
}

void Menu::setupTitle() {
    titleText.setFont(font);
    titleText.setString("Choose Difficulty:");
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color::White);

    titleText.setPosition(
        windowSize.x / 2.f - titleText.getLocalBounds().width / 2.f,
        50
    );
}

void Menu::setupButtons() {
    sf::Vector2f buttonSize(300, 50);
    float centerX = windowSize.x / 2.f - buttonSize.x / 2.f;

    setupButton(easyButton, easyText, "Easy (4x4)",
        centerX, 130, buttonSize, sf::Color(100, 200, 100), 140);
    setupButton(mediumButton, mediumText, "Medium (6x6)",
        centerX, 210, buttonSize, sf::Color(255, 215, 0), 220);
    setupButton(hardButton, hardText, "Hard (8x8)",
        centerX, 290, buttonSize, sf::Color(200, 50, 50), 300);
}

void Menu::setupButton(sf::RectangleShape& button, sf::Text& text,
    const std::string& label, float x, float y,
    const sf::Vector2f& size, const sf::Color& color,
    float textY) {
    button.setSize(size);
    button.setPosition(x, y);
    button.setFillColor(color);

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(
        x + (size.x - text.getLocalBounds().width) / 2.f,
        textY
    );
}

void Menu::updateHover(const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mp(mousePos);

    easyButton.setFillColor(
        easyButton.getGlobalBounds().contains(mp) ?
        sf::Color(120, 220, 120) : sf::Color(100, 200, 100)
    );

    mediumButton.setFillColor(
        mediumButton.getGlobalBounds().contains(mp) ?
        sf::Color(255, 235, 50) : sf::Color(255, 215, 0)
    );

    hardButton.setFillColor(
        hardButton.getGlobalBounds().contains(mp) ?
        sf::Color(220, 70, 70) : sf::Color(200, 50, 50)
    );
}

Level Menu::processClick(float x, float y) {
    if (easyButton.getGlobalBounds().contains(x, y)) {
        return Level::Easy;
    }
    else if (mediumButton.getGlobalBounds().contains(x, y)) {
        return Level::Medium;
    }
    else if (hardButton.getGlobalBounds().contains(x, y)) {
        return Level::Hard;
    }
    return Level::None;
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(menuBackgroundSprite);
    window.draw(titleText);

    window.draw(easyButton);
    window.draw(easyText);

    window.draw(mediumButton);
    window.draw(mediumText);

    window.draw(hardButton);
    window.draw(hardText);
}