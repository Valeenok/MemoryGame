#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "VisualCard.h"
#include "Level.h"
#include "State.h"

class Game {
public:
    Game();
    void run();

private:
    // Основне вікно та карти
    sf::RenderWindow window;
    std::vector<VisualCard> cards;
    VisualCard* first;
    VisualCard* second;

    // Таймери та обмеження
    sf::Clock revealTimer;
    sf::Clock levelClock;
    sf::Time levelTimeLimit;
    int attemptsLeft;
    bool checkingMatch;
    bool isGameOver = false;

    // Стани гри
    Level currentLevel;
    State state;

    // Шрифт
    sf::Font font;

    // UI праворуч (таймер, спроби)
    sf::RectangleShape uiPanel;
    sf::Text timerText;
    sf::Text attemptsText;

    // Елементи меню
    sf::Text titleText, easyText, mediumText, hardText;
    sf::RectangleShape easyButton, mediumButton, hardButton;
    sf::Texture menuBackgroundTexture;
    sf::Sprite menuBackgroundSprite;

    // Елементи завершення гри
    sf::RectangleShape gameOverBackground;
    sf::Text gameOverMessage;
    sf::RectangleShape retryButton;
    sf::Text retryButtonText;
    sf::RectangleShape exitButton;
    sf::Text exitButtonText;

    // Основні функції
    void initializeWindow();
    void initializeUIPanel();
    void initializeTextElements();

    // Меню
    void setupMenu();
    void loadMenuAssets();
    void setupMenuBackground();
    void setupMenuTitle();
    void setupMenuButtons();
    void setupButton(sf::RectangleShape& button, sf::Text& text, const std::string& label,
        float x, float y, const sf::Vector2f& size,
        const sf::Color& color, float textY);
    void updateMenuHover();
    void processMenuClick(float x, float y);

    // Рівень
    void createCards(Level level);
    void resetCardState();
    void setLevelParameters(Level level);
    void initializeLevelUI();
    std::pair<int, int> getGridSize(Level level) const;
    std::vector<int> generateShuffledCardIDs(int rows, int cols);
    void createVisualCards(const std::vector<int>& ids, int rows, int cols, float size);

    // Ігрова логіка
    void handleClick(float x, float y);
    void update();
    void draw();

    // Таймер та спроби
    void startLevelTimer();
    void updateTimerText();
    void updateAttemptsText();

    // Завершення гри
    void setupGameOverMenu();
    void checkGameOverConditions();
    void processGameOverClick(float x, float y);
};
