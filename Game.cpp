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
    initializeUIPanel();
    initializeTextElements();
    setupMenu();
    setupGameOverMenu();
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

void Game::initializeUIPanel() {
    uiPanel.setSize({ 200.f, static_cast<float>(window.getSize().y) });
    uiPanel.setFillColor(sf::Color(50, 50, 50, 200));
    uiPanel.setPosition(window.getSize().x - 200.f, 0);
}

void Game::initializeTextElements() {
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(window.getSize().x - 180.f, 100);

    attemptsText.setFont(font);
    attemptsText.setCharacterSize(24);
    attemptsText.setFillColor(sf::Color::White);
    attemptsText.setPosition(window.getSize().x - 180.f, 150);
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
    updateAttemptsText();
    updateTimerText();
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


void Game::handleClick(float x, float y) {
    if (checkingMatch) return;

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
        updateTimerText();

        if (levelClock.getElapsedTime() >= levelTimeLimit) {
            isGameOver = true;
            gameOverMessage.setString("Time is up!");
        }

        if (checkingMatch && revealTimer.getElapsedTime().asSeconds() > 1) {
            if (first->getId() == second->getId()) {
                first->match();
                second->match();
                attemptsLeft += 2;
            }
            else {
                first->hide();
                second->hide();
                attemptsLeft -= 1;
            }

            updateAttemptsText();

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
        window.draw(menuBackgroundSprite);
        window.draw(titleText);

        window.draw(easyButton);
        window.draw(easyText);

        window.draw(mediumButton);
        window.draw(mediumText);

        window.draw(hardButton);
        window.draw(hardText);
    }
    else {
        for (auto& card : cards)
            card.draw(window);

        // Малюємо UI панель праворуч
        window.draw(uiPanel);
        window.draw(timerText);
        window.draw(attemptsText);

        if (isGameOver) {
            window.draw(gameOverBackground);
            window.draw(gameOverMessage);
            window.draw(retryButton);
            window.draw(retryButtonText);
            window.draw(exitButton);
            window.draw(exitButtonText);
        }
    }

    window.display();
}

void Game::processMenuClick(float x, float y) {
    if (easyButton.getGlobalBounds().contains(x, y)) {
        currentLevel = Level::Easy;
    }
    else if (mediumButton.getGlobalBounds().contains(x, y)) {
        currentLevel = Level::Medium;
    }
    else if (hardButton.getGlobalBounds().contains(x, y)) {
        currentLevel = Level::Hard;
    }
    else {
        return;
    }

    createCards(currentLevel);
    state = State::Playing;
}

void Game::updateMenuHover() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mp(mousePos);

    easyButton.setFillColor(easyButton.getGlobalBounds().contains(mp) ? sf::Color(120, 220, 120) : sf::Color(100, 200, 100));
    mediumButton.setFillColor(mediumButton.getGlobalBounds().contains(mp) ? sf::Color(255, 235, 50) : sf::Color(255, 215, 0));
    hardButton.setFillColor(hardButton.getGlobalBounds().contains(mp) ? sf::Color(220, 70, 70) : sf::Color(200, 50, 50));
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
                    processMenuClick(x, y);
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
            updateMenuHover();
        }

        update();
        draw();
    }
}

void Game::setupMenu() {
    loadMenuAssets();
    setupMenuBackground();
    setupMenuTitle();
    setupMenuButtons();
}

void Game::loadMenuAssets() {
    if (!font.loadFromFile("assets/font/arial.ttf")) {
        std::cerr << "Could not load font!\n";
    }

    if (!menuBackgroundTexture.loadFromFile("assets/menu_background.jpg")) {
        std::cerr << "Could not load menu background image!\n";
    }
}

void Game::setupMenuBackground() {
    menuBackgroundSprite.setTexture(menuBackgroundTexture);
    sf::Vector2u textureSize = menuBackgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    menuBackgroundSprite.setScale(scaleX, scaleY);
}


void Game::setupMenuTitle() {
    titleText.setFont(font);
    titleText.setString("Choose Difficulty:");
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color::White);

    sf::Vector2u windowSize = window.getSize();
    titleText.setPosition(
        windowSize.x / 2.f - titleText.getLocalBounds().width / 2.f,
        50
    );
}

void Game::setupMenuButtons() {
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f buttonSize(300, 50);
    float centerX = windowSize.x / 2.f - buttonSize.x / 2.f;

    setupButton(easyButton, easyText, "Easy (4x4)", centerX, 130, buttonSize, sf::Color(100, 200, 100), 140);
    setupButton(mediumButton, mediumText, "Medium (6x6)", centerX, 210, buttonSize, sf::Color(255, 215, 0), 220);
    setupButton(hardButton, hardText, "Hard (8x8)", centerX, 290, buttonSize, sf::Color(200, 50, 50), 300);
}

void Game::setupButton(sf::RectangleShape& button, sf::Text& text, const std::string& label,
    float x, float y, const sf::Vector2f& size,
    const sf::Color& color, float textY) {
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

void Game::setupGameOverMenu() {
    gameOverBackground.setSize(sf::Vector2f(window.getSize()));
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

void Game::checkGameOverConditions() {
    if (attemptsLeft <= 0) {
        isGameOver = true;
        gameOverMessage.setString("Out of attempts!");
    }

    bool allMatched = std::all_of(cards.begin(), cards.end(), [](const VisualCard& c) {
        return c.matched();
        });

    if (allMatched) {
        isGameOver = true;
        gameOverMessage.setString("You win!");
    }
}

void Game::processGameOverClick(float x, float y) {
    if (retryButton.getGlobalBounds().contains(x, y)) {
        createCards(currentLevel);
        state = State::Playing;
    }
    else if (exitButton.getGlobalBounds().contains(x, y)) {
        state = State::Menu;
    }
    hardText.setString("Hard (8x8)");
    hardText.setCharacterSize(24);
    hardText.setFillColor(sf::Color::Black);
    hardText.setPosition(x + 70, 300);
}
