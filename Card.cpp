#include "Card.h"

Card::Card(int id) : id(id), isRevealed(false), isMatched(false) {}

void Card::reveal() { isRevealed = true; }
void Card::hide() { isRevealed = false; }
void Card::match() { isMatched = true; }

bool Card::revealed() const { return isRevealed; }
bool Card::matched() const { return isMatched; }
int Card::getId() const { return id; }
