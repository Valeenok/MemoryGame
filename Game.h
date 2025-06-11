#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "VisualCard.h"
#include "Level.h"
#include "State.h"
#include "Menu.h"
#include "GameUI.h"
#include "Sound.h"

class Game {
public:
    Game();
    void run();

private:
    Menu menu;
    GameUI ui;
    Sound sound;

    sf::RenderWindow window;
    std::vector<VisualCard> cards;
    VisualCard* first;
    VisualCard* second;

    sf::Clock revealTimer;
    sf::Clock levelClock;
    sf::Time levelTimeLimit;
    int attemptsLeft;
    bool checkingMatch;
    bool isGameOver = false;

    Level currentLevel;
    State state;

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

    void initializeWindow();
    void createCards(Level level);
    void resetCardState();
    void setLevelParameters(Level level);
    void initializeLevelUI();
    std::pair<int, int> getGridSize(Level level) const;
    std::vector<int> generateShuffledCardIDs(int rows, int cols);
    void createVisualCards(const std::vector<int>& ids, int rows, int cols, float size);

    void handleClick(float x, float y);
    int getMaxAttemptsForLevel(Level level) const;
    void update();
    void draw();

    void startLevelTimer();
    void updateTimerText();
    void updateAttemptsText();

    void checkGameOverConditions();
    void processGameOverClick(float x, float y);
};