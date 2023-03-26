#ifndef DECK_HPP
#define DECK_HPP

#include "card.hpp"
#include <list>
#include <array>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <iterator>

class Deck{

private:
std::list<Card> *m_cardsDeck;
public:
    Deck();
    Deck(std::list<Card> *cards);
    ~Deck();
    std::list<Card>* getDeck();
    void setDeck(std::list<Card> *cards);
    void generateDeck();
    void shuffleDeck(const std::string& value);
    void displayDeck();
    void displayDetailedDeck();
    std::list<Card>::iterator findCardById(int id);
    std::list<Card>::iterator findCardByName(const std::string& name);
    std::string createKeyStream(int size);
    void firstStep();
    void secondStep();
    void thirdStep();
    void fourthStep();
    friend bool operator==(Deck& deck1, Deck& deck2);
};

#endif