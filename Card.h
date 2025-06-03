#pragma once

class Card {
protected:
    int id;
    bool isRevealed;
    bool isMatched;

public:
    Card(int id);
    void reveal();
    void hide();
    void match();
    bool revealed() const;
    bool matched() const;
    int getId() const;
};
