#ifndef SOLITAIRETEST_HPP
#define SOLITAIRETEST_HPP

#include "deck.hpp"
#include "encryptanddecrypt.hpp"

class SolitaireTest 
{
    private:
    Deck m_creator;
    Deck m_creator_temp;
    Deck m_correspondant;
    EncryptAndDecrypt m_cad;

    public:
    SolitaireTest();
    ~SolitaireTest();
    void tests();
    void test_generateDeck();
    void test_shuffleDeck();
    void test_setDeck();
    void test_findCardById();
    void test_findCardByName();
    void test_firstStep();
    void test_secondStep();
    void test_thirdStep();
    void test_fourthStep();
    void test_createKeyStream();
};

#endif