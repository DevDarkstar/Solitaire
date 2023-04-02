#include "solitairetest.hpp"

SolitaireTest::SolitaireTest(): m_creator(), m_correspondant(), m_cad()
{}

SolitaireTest::~SolitaireTest()
{}

void SolitaireTest::tests()
{
    test_generateDeck();
    test_shuffleDeck();
    test_setDeck();
    test_findCardById();
    test_findCardByName();
    test_firstStep();  
    test_secondStep();
    test_thirdStep();
    test_fourthStep(); 
    test_createKeyStream();
    test_correctMessage();
    test_setCodingKeyString();
    test_encryptMessage();
    test_decryptMessage();
    test_addDotsAndSpaces();
}

void SolitaireTest::test_generateDeck()
{
    //On récupère le nom de la première carte du deck généré
    std::list<Card> *cards = m_creator.getDeck();
    std::list<Card>::iterator it = cards->begin();
    std::string name_first = (*it).getName();
    //Ainsi que le nom de la dernière carte
    std::advance(it, 53);
    std::string name_last = (*it).getName();

    //On vérifie que la première carte générée est l'as de trèfle (AT)
    //Et la dernière carte le joker rouge (RedJoker)
    if(name_first.compare("AT") == 0 && name_last.compare("RedJoker") == 0)
    {
        std::cout << "getDeck OK\n";
        std::cout << "generateDeck OK\n";
        std::cout << "getName OK\n";
    }
    else
    {
        std::cout << "generateDeck FALSE" << std::endl;
        exit(1);
    }
}

void SolitaireTest::test_shuffleDeck()
{
    m_creator.shuffleDeck("1");
    //m_creator.displayDeckWithNumber();
    //m_creator.displayDeck();
    //On récupère le numéro de la première carte du deck généré
    std::list<Card> *cards = m_creator.getDeck();
    std::list<Card>::iterator it = cards->begin();
    int id_first = (*it).getId();
    //Ainsi que le numéro de la dernière carte
    std::advance(it, 53);
    int id_last = (*it).getId();

    //On vérifie que la première carte générée est le 9 de coeur (9H) -> id = 35
    //Et la dernière carte le 5 de carreau (5C) -> id = 18
    if(id_first == 35 && id_last == 18)
    {
        std::cout << "shuffleDeck OK\n";
        std::cout << "getId OK\n";
    }
    else
    {
        std::cout << "shuffleDeck FALSE" << std::endl;
        exit(1);
    }
}

void SolitaireTest::test_setDeck()
{
    m_correspondant.setDeck(m_creator.getDeck());
    if(m_correspondant == m_creator)
    {
        std::cout << "setDeck OK\n";
    }
    else
    {
        std::cout << "setDeck FALSE" << std::endl;
        exit(1);
    }
}

void SolitaireTest::test_findCardById()
{
    //On récupère deux cartes du deck: une avec l'id 6 et l'autre avec l'id 41
    std::list<Card>::iterator card1 = m_creator.findCardById(6);
    std::list<Card>::iterator card2 = m_creator.findCardById(41);

    //Puis, on vérifie si les deux cartes sont bien les bonnes:
    //id = 6 -> 6T et id = 41 -> 2P
    if((*card1).getName().compare("6T") == 0 && (*card2).getName().compare("2P") == 0)
    {
        std::cout << "findCardById OK\n";
    }
    else
    {
        std::cout << "findCardById FALSE\n";
        exit(1);
    }
}

void SolitaireTest::test_findCardByName()
{
    //On récupère deux cartes du deck: une avec le nom AH (As de coueur) et l'autre avec le nom 10T (10 de trèfle)
    std::list<Card>::iterator card1 = m_creator.findCardByName("AH");
    std::list<Card>::iterator card2 = m_creator.findCardByName("10T");

    //Puis, on vérifie si les deux cartes sont bien les bonnes:
    //nom = AH -> id = 27 et nom = 10T -> id = 10
    if((*card1).getId() == 27 && (*card2).getId() == 10)
    {
        std::cout << "findCardByName OK\n";
    }
    else
    {
        std::cout << "findCardByName FALSE\n";
        exit(1);
    }
}

void SolitaireTest::test_firstStep()
{
    //On fait reculer le joker noir d'une place dans le deck (par défaut il est à la position 5 dans le deck)
    //On récupère le deck
    std::list<Card>* cards = m_creator.getDeck(); 
    m_creator.firstStep();
    //On crée un itérateur pour traquer le joker noir
    std::list<Card>::iterator it = cards->begin();
    std::advance(it, 5);
    if((*it).getName().compare("BlackJoker") == 0)
    {
        //On teste une nouvelle fois
        m_creator.firstStep();
        it = cards->begin();
        std::advance(it, 6);
        if((*it).getName().compare("BlackJoker") == 0)
        {
            std::cout << "firstStep OK\n";
        }
        else
        {
            std::cout << "firstStep FALSE\n";
            exit(1);
        }
    }
    else
    {
        std::cout << "firstStep FALSE\n";
        exit(1);
    }
}

void SolitaireTest::test_secondStep()
{
    //On fait reculer le joker rouge de deux places dans le deck (par défaut il est à la position 48 dans le deck)
    //On récupère le deck
    std::list<Card>* cards = m_creator.getDeck();
    m_creator.secondStep();
    //On crée un itérateur pour traquer le joker noir
    std::list<Card>::iterator it = cards->begin();
    std::advance(it, 49);
    if((*it).getName().compare("RedJoker") == 0)
    {
        //On teste une nouvelle fois
        m_creator.secondStep();
        m_creator.secondStep();
        m_creator.secondStep();
        //Il doit être en troisième position maintenant
        it = cards->begin();
        std::advance(it, 2);
        if((*it).getName().compare("RedJoker") == 0)
        {
            std::cout << "secondStep OK\n";
        }
        else
        {
            std::cout << "secondStep FALSE\n";
            exit(1);
        }
    }
    else
    {
        std::cout << "secondStep FALSE\n";
        exit(1);
    }
}

void SolitaireTest::test_thirdStep()
{
    //On fait une double coupe par rapport aux deux jokers (le joker rouge est en troisième position)
    //et le joker noir est en huitième position
    //Si la coupe par fragments fonctionne, on doit avoir la carte 3H en fin de deck 
    //et la carte DH au début du deck
    std::list<Card>* cards = m_creator.getDeck();
    m_creator.thirdStep();
    //On récupère la première carte du deck
    std::list<Card>::iterator it = cards->begin();
    //Si cette carte correspond bien à la carte DH (Dame de coeur)
    if((*it).getName().compare("DH") == 0)
    {
        //On récupère la dernière carte du deck
        std::advance(it, 53);
        //S'il s'agit bien du 3 de coeur (3H), la fonction est bien fonctionnelle
        if((*it).getName().compare("3H") == 0)
        {
            std::cout << "thirdStep OK\n";
        }
        else
        {
            std::cout << "thirdStep FALSE\n";
            exit(1);
        }
    }
    else
    {
        std::cout << "thirdStep FALSE\n";
        exit(1);
    }
}

void SolitaireTest::test_fourthStep()
{
    //Coupe suivant le numéro de la dernière carte du deck (ici 29 = 3H)
    //On récupère la carte située à la position 29 dans le deck
    std::list<Card>* cards = m_creator.getDeck();
    std::list<Card>::iterator it = cards->begin();
    std::advance(it, 28);
    //Et la dernière carte
    Card last_card = cards->back();
    //On réalise la quatrième étape
    m_creator.fourthStep();
    //Si l'étape a bien fonctionné, la carte à la position 29 doit être 
    //à l'avant-dernière position dans le deck maintenant
    //et la dernière carte doit rester la même
    //On récupère l'avant-dernière carte
    Card penultimateCard = *(std::prev(std::prev(cards->end())));
    //et la dernière
    Card new_last_card = cards->back();
    if(last_card == new_last_card && (*it) == penultimateCard)
    {
        std::cout << "fourthStep OK\n";
    }
    else
    {
        std::cout << "fourthStep FALSE\n";
        exit(1);
    }
}

void SolitaireTest::test_createKeyStream()
{
    //Lecture d'une lettre pseudo-aléatoire
    std::list<Card> *cards = m_creator.getDeck();
    std::string result = m_creator.createKeyStream(1);
    //On lit la première carte du deck m_creator après les quatre premières étapes (ici VH -> Valet de coeur)
    //donc un numéro de carte de 37
    Card card_first = *(cards->begin());
    if(card_first.getName().compare("VH") == 0)
    {
        //On se déplace de 2 et on lit la troisième carte du deck (n + 1) -> ici (RP = Roi de pic)
        //Par conséquent la valeur d'une keystream de taille 1 sur ce deck donnera la 26ème
        //lettre de l'alphabet soit "Z"
        std::list<Card>::iterator it = cards->begin();
        std::advance(it, card_first.getId());
        if((*it).getName().compare("RP") == 0)
        {
            if(result.compare("Z") == 0)
            {
                std::cout << "createKeyStream OK\n";
            }
            else
            {
                std::cout << "createKeyStream FALSE\n";
                exit(1);
            }
        }
        else
        {
            std::cout << "createKeyStream FALSE\n";
            exit(1);
        }
    }
    else
    {
        std::cout << "createKeyStream FALSE\n";
        exit(1);
    }
}

void SolitaireTest::test_correctMessage()
{
    std::string message = "Un message.";
    std::string correctedMessage = EncryptAndDecrypt::correctMessage(message);

    if(correctedMessage.compare("Unszmessagestop") == 0)
    {
        std::cout << "correctMessage OK\n";
    }
    else
    {
        std::cout << "correctMessage FALSE\n";
        exit(1);
    }
}

void SolitaireTest::test_setCodingKeyString()
{
    //On crée un message pour tester les fonctions de la classe EncryptAndDecrypt
    std::string message = "Unszmessagestop";
    std::string key = m_creator.createKeyStream(message.size());
    m_cad.setCodingKeyString(key);
    std::string keyString = m_cad.getCodingKey();
    if(keyString.compare("PMKKAQVQKSAMVPP") == 0)
    {
        int numbers[] = {16, 13, 11, 11, 1, 17, 22, 17, 11, 19, 1, 13, 22, 16, 16};
        std::list<int> *keyNumber = m_cad.getCodingKeyNumbers();
        bool areTheSame = true;
        int i = 0;
        std::list<int>::iterator it = keyNumber->begin();
        while(areTheSame && i < message.size() && it != keyNumber->end())
        {
            if(numbers[i] != *it)
                areTheSame = false;
            else
            {
                i++;
                ++it;
            }
        }
        if(areTheSame)
        {
            std::cout << "getCodingKey OK\n";
            std::cout << "getCodingKeyNumbers OK\n";
            std::cout << "setCodingKeyString OK\n";
            std::cout << "setCodingKeyNumbers OK\n";
            std::cout << "convertKeyToNumbers OK\n";
        }
        else
        {
            std::cout << "setCodingKeyString FALSE\n";
            std::cout << "setCodingKeyNumbers FALSE\n";
            exit(1);
        }
    }
    else
    {
        std::cout << "setCodingKeyString FALSE\n";
        std::cout << "setCodingKeyNumbers FALSE\n";
        exit(1);
    }
}

void SolitaireTest::test_encryptMessage()
{
    std::string message = "Unszmessagestop";
    std::string encryptedMessage = m_cad.encryptMessage(message);  
    if(encryptedMessage.compare("KADKNVOJLZFFPEF") == 0)
    {
        std::cout << "encryptMessage OK\n";
    }
    else
    {
        std::cout << "encryptMessage FALSE\n";
        exit(1);
    }
}

void SolitaireTest::test_decryptMessage()
{
    std::string cryptedMessage = "KADKNVOJLZFFPEF";
    std::string decryptedMessage = m_cad.decryptMessage(cryptedMessage);  
    if(decryptedMessage.compare("Un message.") == 0)
    {
        std::cout << "decryptMessage OK\n";
        std::cout << "convertKeyToString OK\n";
    }
    else
    {
        std::cout << decryptedMessage << "\n";
        std::cout << "decryptMessage FALSE\n";
        exit(1);
    }
}

void SolitaireTest::test_addDotsAndSpaces()
{
    std::string message = "unszMeSsaGestop";
    message = m_cad.addDotsAndSpaces(message);
    if(message.compare("un message.") == 0)
    {
        std::cout << "replaceStopByDots OK\n";
    }
    else
    {
        std::cout << "replaceStopByDots FALSE\n";
        exit(1);
    }
}
