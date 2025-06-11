#include "GameUI.h"
#include <iostream>

GameUI::GameUI() {}

void GameUI::initialize(const sf::Vector2u& windowSize, const std::string& fontPath) {
    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Could not load font from " << fontPath << std::endl;
    }

    // UI панель
    uiPanel.setSize({ 200.f, static_cast<float>(windowSize.y) });
    uiPanel.setFillColor(sf::Color(50, 50, 50, 200));
    uiPanel.setPosition(windowSize.x - 200.f, 0);
    setupInGameExitButton();

    // Таймер
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(windowSize.x - 180.f, 100);

    // Спроби
    attemptsText.setFont(font);
    attemptsText.setCharacterSize(24);
    attemptsText.setFillColor(sf::Color::White);
    attemptsText.setPosition(windowSize.x - 180.f, 150);
}

void GameUI::updateTimer(sf::Time remainingTime) {
    if (remainingTime < sf::Time::Zero) remainingTime = sf::Time::Zero;

    int seconds = static_cast<int>(remainingTime.asSeconds()) % 60;
    int minutes = static_cast<int>(remainingTime.asSeconds()) / 60;

    timerText.setString("Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
}

void GameUI::updateAttempts(int attemptsLeft) {
    attemptsText.setString("Attempts: " + std::to_string(attemptsLeft));
}

void GameUI::setupGameOverMenu(const sf::Vector2u& windowSize) {
    gameOverBackground.setSize(sf::Vector2f(windowSize));
    gameOverBackground.setFillColor(sf::Color(0, 0, 0, 150));

    gameOverMessage.setFont(font);
    gameOverMessage.setCharacterSize(36);
    gameOverMessage.setFillColor(sf::Color::White);
    gameOverMessage.setPosition(150, 200);

    retryButton.setSize({ 200, 50 });
    retryButton.setFillColor(sf::Color(100, 200, 100));
    retryButton.setPosition(150, 300);

    retryButtonText.setFont(font);
    retryButtonText.setString("Try Again");
    retryButtonText.setCharacterSize(24);
    retryButtonText.setFillColor(sf::Color::Black);
    retryButtonText.setPosition(175, 310);

    exitButton.setSize({ 200, 50 });
    exitButton.setFillColor(sf::Color(200, 50, 50));
    exitButton.setPosition(400, 300);

    exitButtonText.setFont(font);
    exitButtonText.setString("Exit to Menu");
    exitButtonText.setCharacterSize(24);
    exitButtonText.setFillColor(sf::Color::Black);
    exitButtonText.setPosition(410, 310);
}

void GameUI::setGameOverMessage(const std::string& message) {
    gameOverMessage.setString(message);
}

void GameUI::draw(sf::RenderWindow& window, bool isGameOver) {
    window.draw(uiPanel);
    window.draw(timerText);
    window.draw(attemptsText);
    window.draw(inGameExitButton);
    window.draw(inGameExitButtonText);

    if (isGameOver) {
        window.draw(gameOverBackground);
        window.draw(gameOverMessage);
        window.draw(retryButton);
        window.draw(retryButtonText);
        window.draw(exitButton);
        window.draw(exitButtonText);
    }
}

bool GameUI::isRetryButtonClicked(float x, float y) const {
    return retryButton.getGlobalBounds().contains(x, y);
}

bool GameUI::isExitButtonClicked(float x, float y) const {
    return exitButton.getGlobalBounds().contains(x, y);
}

void GameUI::setupInGameExitButton() {
    inGameExitButton.setSize({ 160, 40 });
    inGameExitButton.setFillColor(sf::Color(200, 50, 50));
    inGameExitButton.setPosition(uiPanel.getPosition().x + 20, 250);

    inGameExitButtonText.setFont(font);
    inGameExitButtonText.setString("Exit to Menu");
    inGameExitButtonText.setCharacterSize(20);
    inGameExitButtonText.setFillColor(sf::Color::Black);
    inGameExitButtonText.setPosition(inGameExitButton.getPosition().x + 10, inGameExitButton.getPosition().y + 10);
}

bool GameUI::isInGameExitButtonClicked(float x, float y) const {
    return inGameExitButton.getGlobalBounds().contains(x, y);
}

void GameUI::updateHover(const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mp(mousePos);

    if (inGameExitButton.getGlobalBounds().contains(mp)) {
        inGameExitButton.setFillColor(sf::Color(220, 70, 70));
    }
    else {
        inGameExitButton.setFillColor(sf::Color(200, 50, 50));
    }
}
