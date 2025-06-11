#include "Game.h"
#include <algorithm>
#include <random>
#include <iostream>

Game::Game()
    : window(sf::VideoMode(1, 1), "Find the Pair"),
    first(nullptr), second(nullptr), checkingMatch(false),
    state(State::Menu)
{
    initializeWindow();

    ui.initialize(window.getSize(), "assets/font/arial.ttf");
    ui.setupGameOverMenu(window.getSize());

    if (!menu.initialize(window.getSize())) {
        std::cerr << "Failed to initialize menu!" << std::endl;
    }

    VisualCard::loadFont("assets/font/arial.ttf");
}


void Game::initializeWindow() {
    const int numRows = 6;
    const int numCols = 6;
    const int cardSize = 100;
    const int padding = 10;
    const int sidePanelWidth = 200;

    int windowWidth = numCols * (cardSize + padding) + padding + sidePanelWidth;
    int windowHeight = numRows * (cardSize + padding) + padding;

    window.create(sf::VideoMode(windowWidth, windowHeight), "Find the Pair");
}


void Game::createCards(Level level) {
    resetCardState();
    setLevelParameters(level);
    initializeLevelUI();

    int rows, cols;
    std::tie(rows, cols) = getGridSize(level);

    float gameAreaWidth = window.getSize().x - 200.f;
    float size = gameAreaWidth / cols - 5.f;

    std::vector<int> ids = generateShuffledCardIDs(rows, cols);
    createVisualCards(ids, rows, cols, size);
}

void Game::resetCardState() {
    cards.clear();
    first = nullptr;
    second = nullptr;
    checkingMatch = false;
    isGameOver = false;
}

void Game::setLevelParameters(Level level) {
    switch (level) {
    case Level::Easy:
        levelTimeLimit = sf::seconds(90);
        attemptsLeft = 6;
        break;
    case Level::Medium:
        levelTimeLimit = sf::seconds(120);
        attemptsLeft = 10;
        break;
    case Level::Hard:
        levelTimeLimit = sf::seconds(210);
        attemptsLeft = 20;
        break;
    }
}

void Game::initializeLevelUI() {
    startLevelTimer();
    ui.updateAttempts(attemptsLeft);
    ui.updateTimer(levelTimeLimit);
}

std::pair<int, int> Game::getGridSize(Level level) const {
    switch (level) {
    case Level::Easy:
        return { 4, 4 };
    case Level::Medium:
        return { 6, 6 };
    case Level::Hard:
        return { 8, 8 };
    default:
        return { 4, 4 };
    }
}

std::vector<int> Game::generateShuffledCardIDs(int rows, int cols) {
    std::vector<int> ids;
    for (int i = 0; i < (rows * cols) / 2; ++i) {
        ids.push_back(i);
        ids.push_back(i);
    }
    std::shuffle(ids.begin(), ids.end(), std::mt19937{ std::random_device{}() });
    return ids;
}

void Game::createVisualCards(const std::vector<int>& ids, int rows, int cols, float size) {
    for (int i = 0; i < rows * cols; ++i) {
        float x = (i % cols) * (size + 5.f) + 5;
        float y = (i / cols) * (size + 5.f) + 5;

        VisualCard card(ids[i], x, y, size);

        std::string frontPath = "assets/cards/Card" + std::to_string(ids[i] + 1) + ".png";
        std::string backPath = "assets/cards/back.png";

        if (!card.loadTextures(frontPath, backPath)) {
            std::cerr << "Unable to download texture for card with ID = " << ids[i] << std::endl;
        }

        cards.push_back(card);
    }
}

int Game::getMaxAttemptsForLevel(Level level) const {
    switch (level) {
    case Level::Easy:
        return 6;
    case Level::Medium:
        return 10;
    case Level::Hard:
        return 20;
    default:
        return 6;
    }
}

void Game::handleClick(float x, float y) {
    if (checkingMatch) return;

    if (ui.isInGameExitButtonClicked(x, y)) {
        state = State::Menu;
        return;
    }

    for (auto& card : cards) {
        if (card.contains({ x, y }) && !card.revealed() && !card.matched()) {
            card.reveal();
            card.updateVisualState();

            if (!first) {
                first = &card;
            }
            else if (!second && &card != first) {
                second = &card;
                checkingMatch = true;
                revealTimer.restart();
            }
            break;
        }
    }
}


void Game::update() {
    if (state == State::Playing && !isGameOver) {
        sf::Time elapsed = levelClock.getElapsedTime();
        sf::Time remaining = levelTimeLimit - elapsed;
        ui.updateTimer(remaining);

        if (remaining <= sf::Time::Zero) {
            isGameOver = true;
            ui.setGameOverMessage("Time is up!");
        }

        if (checkingMatch && revealTimer.getElapsedTime().asSeconds() > 1) {
            if (first->getId() == second->getId()) {
                first->match();
                second->match();
                int maxAttempts = getMaxAttemptsForLevel(currentLevel);
                attemptsLeft = std::min(attemptsLeft + 2, maxAttempts);
            }
            else {
                first->hide();
                second->hide();
                attemptsLeft -= 1;
            }

            ui.updateAttempts(attemptsLeft);

            first->updateVisualState();
            second->updateVisualState();
            first = second = nullptr;
            checkingMatch = false;

            checkGameOverConditions();
        }
    }
}

void Game::draw() {
    window.clear();

    if (state == State::Menu) {
        menu.draw(window);
    }
    else {
        for (auto& card : cards)
            card.draw(window);

        ui.draw(window, isGameOver);
    }

    window.display();
}

void Game::startLevelTimer() {
    levelClock.restart();
}

void Game::updateTimerText() {
    sf::Time elapsed = levelClock.getElapsedTime();
    sf::Time remaining = levelTimeLimit - elapsed;
    if (remaining < sf::Time::Zero) remaining = sf::Time::Zero;

    int seconds = static_cast<int>(remaining.asSeconds()) % 60;
    int minutes = static_cast<int>(remaining.asSeconds()) / 60;

    timerText.setString("Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
}

void Game::updateAttemptsText() {
    attemptsText.setString("Attempts: " + std::to_string(attemptsLeft));
}


void Game::checkGameOverConditions() {
    if (attemptsLeft <= 0) {
        isGameOver = true;
        ui.setGameOverMessage("Out of attempts!");
    }

    bool allMatched = std::all_of(cards.begin(), cards.end(), [](const VisualCard& c) {
        return c.matched();
        });

    if (allMatched) {
        isGameOver = true;
        ui.setGameOverMessage("You win!");
    }
}

void Game::processGameOverClick(float x, float y) {
    if (ui.isRetryButtonClicked(x, y)) {
        createCards(currentLevel);
        state = State::Playing;
    }
    else if (ui.isExitButtonClicked(x, y)) {
        state = State::Menu;
    }
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                float x = event.mouseButton.x;
                float y = event.mouseButton.y;

                if (state == State::Menu) {
                    Level selectedLevel = menu.processClick(x, y);
                    if (selectedLevel != Level::None) {
                        currentLevel = selectedLevel;
                        createCards(currentLevel);
                        state = State::Playing;
                    }
                }
                else if (isGameOver) {
                    processGameOverClick(x, y);
                }
                else {
                    handleClick(x, y);
                }
            }
        }
        if (state == State::Menu) {
            menu.updateHover(window);
        }
        else if (state == State::Playing && !isGameOver) {
            ui.updateHover(window);
        }

        update();
        draw();
    }
}