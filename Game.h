#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "VisualCard.h"

enum class State { Menu, Playing };

class Game {
public:
    enum class Level { Easy, Medium, Hard };

private:
    sf::RenderWindow window;
    std::vector<VisualCard> cards;
    VisualCard* first;
    VisualCard* second;
    sf::Clock revealTimer;
    bool checkingMatch;

    Level currentLevel;
    State state;

    sf::Font font;
    sf::Text titleText, easyText, mediumText, hardText;

    void setupMenu();
    void createCards(Level level);
    void handleClick(float x, float y);
    void update();
    void draw();
    void updateMenuHover();


    void processMenuClick(float x, float y);

    sf::RectangleShape background;

    sf::RectangleShape easyButton;
    sf::RectangleShape mediumButton;
    sf::RectangleShape hardButton;
    sf::Texture menuBackgroundTexture;
    sf::Sprite menuBackgroundSprite;

public:
    Game();
    void run();
};