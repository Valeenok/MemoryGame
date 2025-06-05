#include "Game.h"
#include <algorithm>
#include <random>
#include <iostream>

Game::Game()
    : window(sf::VideoMode(700, 700), "Find the Pair"),
    first(nullptr), second(nullptr), checkingMatch(false),
    state(State::Menu)
{
    VisualCard::loadFont("assets/font/arial.ttf");
    setupMenu();
}

void Game::createCards(Level level) {
    cards.clear();
    int rows, cols;

    if (level == Level::Easy) { rows = cols = 4; }
    else if (level == Level::Medium) { rows = cols = 6; }
    else { rows = cols = 8; }

    float size = 600.f / cols - 5.f;
    std::vector<int> ids;

    for (int i = 0; i < (rows * cols) / 2; ++i) {
        ids.push_back(i);
        ids.push_back(i);
    }

    std::shuffle(ids.begin(), ids.end(), std::mt19937{ std::random_device{}() });

    for (int i = 0; i < rows * cols; ++i) {
        float x = (i % cols) * (size + 5.f) + 5;
        float y = (i / cols) * (size + 5.f) + 5;

        VisualCard card(ids[i], x, y, size);

        // Формуємо шлях до зображень (Card1.png, Card2.png, ...)
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
    if (checkingMatch && revealTimer.getElapsedTime().asSeconds() > 1) {
        if (first->getId() == second->getId()) {
            first->match();
            second->match();
        }
        else {
            first->hide();
            second->hide();
        }

        first->updateVisualState();
        second->updateVisualState();
        first = second = nullptr;
        checkingMatch = false;
    }
}

void Game::draw() {
    window.clear();

    if (state == State::Menu) {
        window.draw(titleText);

    }
    else {
        for (auto& card : cards)
            card.draw(window);
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

    // Easy
    if (easyButton.getGlobalBounds().contains(mp))
        easyButton.setFillColor(sf::Color(120, 220, 120));
    else
        easyButton.setFillColor(sf::Color(100, 200, 100));

    // Medium
    if (mediumButton.getGlobalBounds().contains(mp))
        mediumButton.setFillColor(sf::Color(255, 235, 50));
    else
        mediumButton.setFillColor(sf::Color(255, 215, 0));

    // Hard
    if (hardButton.getGlobalBounds().contains(mp))
        hardButton.setFillColor(sf::Color(220, 70, 70));
    else
        hardButton.setFillColor(sf::Color(200, 50, 50));
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

                if (state == State::Menu)
                    processMenuClick(x, y);
                else
                    handleClick(x, y);
            }
        }

        if (state == State::Menu)
            updateMenuHover();

        update();
        draw();
    }
}


void Game::setupMenu() {
    font.loadFromFile("assets/font/arial.ttf");

    background.setSize(sf::Vector2f(window.getSize()));

    // Текст заголовку
    titleText.setFont(font);
    titleText.setString("Choose Difficulty:");
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(170, 50);

    // Розміри кнопок
    sf::Vector2f buttonSize(300, 50);
    float x = 150;

    easyButton.setSize(buttonSize);
    easyButton.setPosition(x, 130);
    easyButton.setFillColor(sf::Color(100, 200, 100));
    easyText.setFont(font);
    easyText.setString("Easy (4x4)");
    easyText.setCharacterSize(24);
    easyText.setFillColor(sf::Color::Black);
    easyText.setPosition(x + 70, 140);

    mediumButton.setSize(buttonSize);
    mediumButton.setPosition(x, 210);
    mediumButton.setFillColor(sf::Color(255, 215, 0));
    mediumText.setFont(font);
    mediumText.setString("Medium (6x6)");
    mediumText.setCharacterSize(24);
    mediumText.setFillColor(sf::Color::Black);
    mediumText.setPosition(x + 60, 220);

    hardButton.setSize(buttonSize);
    hardButton.setPosition(x, 290);
    hardButton.setFillColor(sf::Color(200, 50, 50));
    hardText.setFont(font);
    hardText.setString("Hard (8x8)");
    hardText.setCharacterSize(24);
    hardText.setFillColor(sf::Color::Black);
    hardText.setPosition(x + 70, 300);
}
