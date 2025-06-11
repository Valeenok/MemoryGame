#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class GameUI {
public:
    GameUI();

    void initialize(const sf::Vector2u& windowSize, const std::string& fontPath);
    void updateTimer(sf::Time remainingTime);
    void updateAttempts(int attemptsLeft);
    void draw(sf::RenderWindow& window, bool isGameOver);
    void setupGameOverMenu(const sf::Vector2u& windowSize);
    void setGameOverMessage(const std::string& message);

    bool isRetryButtonClicked(float x, float y) const;
    bool isExitButtonClicked(float x, float y) const;

    void setupInGameExitButton();
    bool isInGameExitButtonClicked(float x, float y) const;
    void updateHover(const sf::RenderWindow& window);

private:
    sf::Font font;

    sf::RectangleShape uiPanel;
    sf::Text timerText;
    sf::Text attemptsText;

    sf::RectangleShape gameOverBackground;
    sf::Text gameOverMessage;
    sf::RectangleShape retryButton;
    sf::Text retryButtonText;
    sf::RectangleShape exitButton;
    sf::Text exitButtonText;

    sf::RectangleShape inGameExitButton;
    sf::Text inGameExitButtonText;
};
