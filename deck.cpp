#include "deck.hpp"

Deck::Deck(): m_cardsDeck(new std::list<Card>())
{
    this->generateDeck();
}

Deck::~Deck()
{
    this->m_cardsDeck->clear();
    this->m_cardsDeck = nullptr;
    if(this->m_cardsDeck == nullptr)
        std::cout << "liste supprimee" << std::endl;
}

void Deck::generateDeck()
{
    //Création d'un tableau contenant les noms des cartes
    std::string cardNames[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "V", "D", "R"};
    //Et un pour les couleurs
    //T -> Trèfle; C -> Carreau; H -> Coeur; P -> Pique
    std::string colors[] = {"T", "C", "H", "P"};
    //On crée les cartes à insérer dans le paquet de cartes
    //On crée un compteur qui correspond aux numéros des cartes suivant l'ordre du Bridge
    int id = 1;
    for(int i = 0; i < sizeof(colors)/sizeof(std::string); i++)
    {
        for(int j = 0; j < sizeof(cardNames)/sizeof(std::string); j++)
        {
            //On crée le nom de la carte
            std::string name = cardNames[j] + colors[i];
            //et on l'ajoute au paquet de cartes
            this->m_cardsDeck->push_back(Card(id, name));
            id++;
        }
    }
    //Ajout des deux jokers dans le paquet de cartes
    this->m_cardsDeck->push_back(Card(id, "BlackJoker"));
    id++;
    this->m_cardsDeck->push_back(Card(id, "RedJoker"));
}

void Deck::resetDeck()
{
    this->m_cardsDeck->clear();
    this->m_cardsDeck = nullptr;
    this->m_cardsDeck = new std::list<Card>();
    this->generateDeck();
    this->shuffleDeck();
}

void Deck::shuffleDeck()
{
    //Création d'un tableau contenant les numéros des cartes du paquet
    std::array<int,54> idCards {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54};
    //Création de la seed pour le mélange aléatoire du paquet de cartes
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    //mélange des numéros des cartes du paquet
    std::shuffle(idCards.begin(), idCards.end(), std::default_random_engine(seed));

    for(auto& i: idCards)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    //On crée un deck temporaire qui va contenir le deck mélangé
    std::list<Card> *tempDeck = new std::list<Card>();
    //On parcourt le tableau des numéros de cartes mélangé
    for(std::array<int,54>::iterator it = idCards.begin(); it != idCards.end(); ++it)
    {
        //On récupère la position de la carte dans le deck m_cardsDeck au numéro correspondant dans le tableau idCards
        std::list<Card>::iterator card = this->findCardById(*it);
        //On ajoute la carte correspondante dans le deck temporaire
        tempDeck->push_back(*card);
        //Et on la supprime dans le deck initial
        this->m_cardsDeck->erase(card);
    }

    //On fait en sorte que le deck initial soit vide
    this->m_cardsDeck->clear();
    this->m_cardsDeck = nullptr;
    //On stocke l'adresse du deck temporaire dans l'attribut de la classe Deck
    this->m_cardsDeck = tempDeck;

    //Par convention, les deux jokers ont le même numéro 53, donc on modifie celui du joker rouge (par défaut à 54 pour le mélange des cartes du deck)
    std::list<Card>::iterator redJoker = this->findCardById(54);
    (*redJoker).setId(53);

    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "========== Deck apres melange ==========" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    this->displayDeck();
}

void Deck::displayDeck()
{
    for(std::list<Card>::iterator card = this->m_cardsDeck->begin(); card != this->m_cardsDeck->end(); ++card)
    {
        std::cout << card->getName() << " ";
    }
    std::cout << std::endl;
}

void Deck::displayDetailedDeck()
{
    if(this->m_cardsDeck == nullptr)
    {
        std::cout << "erreur -> le deck revoie un pointeur null" << std::endl;
        exit(1);
    }

    for(std::list<Card>::iterator card = this->m_cardsDeck->begin(); card != this->m_cardsDeck->end(); ++card)
    {
        std::cout << "numero:" << card->getId() << " -> carte:" << card->getName() << " || ";
    }
    std::cout << std::endl;
}

std::list<Card>::iterator Deck::findCardById(int id)
{
    std::list<Card>::iterator it = this->m_cardsDeck->begin();
    bool isFound = false;
    while(it != this->m_cardsDeck->end() && !isFound)
    {
        //On récupère le numéro de la carte
        int idCard = (*it).getId();
        //Et on le compare avec celui de la carte à rechercher
        if(idCard == id)
            isFound = true;
        else
            ++it;
    }
    return it;
}

std::list<Card>::iterator Deck::findCardByName(const std::string& name)
{
    std::list<Card>::iterator it = this->m_cardsDeck->begin();
    bool isFound = false;
    while(it != this->m_cardsDeck->end() && !isFound)
    {
        //On récupère le nom de la carte
        std::string cardName = (*it).getName();
        //et on le compare avec celui de la carte à rechercher
        if(cardName.compare(name) == 0)
            isFound = true;
        else
            ++it;
    }
    return it;
}

std::string Deck::createKeyStream(int size)
{
    //On crée une chaine de caractères qui contiendra la clé servant à coder le message
    std::string keyStream = "";
    //ainsi qu'un chaine contenant l'alphabet
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //On crée un compteur sur la taille du mot passé en paramètre
    int count = 0;
    do{
        //On crée un booléen pour savoir si la carte obtenue à la cinquième étape est ou non un joker
        bool isAJoker;
        do{
            isAJoker = false;
            //On effectue les quatre premières opérations pour obtenir le flux de clefs
            this->firstStep();
            this->secondStep();
            this->thirdStep();
            this->fourthStep();
            //Cinquième étape -> lecture d'une lettre pseudo-aléatoire
            std::cout << "-----------------------------------------------------------------------" << std::endl;
            std::cout << "=== Cinquieme etape -> lecture d'une lettre pseudo-aleatoire ===" << std::endl;
            //On récupère le numéro de la première carte du deck
            int n = this->m_cardsDeck->front().getId();
            std::cout << "numero de la premiere carte du deck: " << n << std::endl;
            //On crée un itérateur que l'on fait pointer sur la n-ième carte du deck
            std::list<Card>::iterator it = this->m_cardsDeck->begin();
            std::advance(it, n);
            //On récupère le numéro de la n-ième carte du deck
            int m = (*it).getId();
            std::cout << "numero de la " << n+1 << "-ieme carte du deck: " << m << std::endl;
            //Si c'est un joker (numéro de carte = 53), on recommence une opération complète de mélange (on repart à la première étape)
            if(m == 53)
                isAJoker = true;
            else
            {
                //Si le numéro de carte dépasse 26, on le soustrait par 26
                if(m > 26)
                    m -= 26;
                //et on l'ajoute à la liste de flux de clés
                std::cout << "Ajout de la cle de flux " << m << "." << std::endl;
                keyStream += alphabet[m-1];
            }

        }while(isAJoker);

        //On incrémente le compteur
        count++;
    }while(count < size);

    return keyStream;
}

void Deck::firstStep()
{
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "=== Premiere etape -> On fait reculer le joker noir d'une place ===" << std::endl;
    

    //On récupère la position du joker noir
    std::list<Card>::iterator blackJokerPos = this->findCardByName("BlackJoker");
    //On récupère la dernière carte du deck
    Card lastCard = this->m_cardsDeck->back();

    //Si le joker noir est la dernière carte du deck
    if((*blackJokerPos).getId() == lastCard.getId())
    {
        //On stocke le joker noir dans une variable temporaire
        Card tempJoker = *blackJokerPos;
        //On supprime le joker noir de sa position actuelle
        this->m_cardsDeck->erase(blackJokerPos);
        //et on l'insère à la position 2 dans le deck
        std::list<Card>::iterator newPos = this->m_cardsDeck->begin();
        std::advance(newPos, 1);
        this->m_cardsDeck->insert(newPos, tempJoker);
    }
    //Sinon le joker noir n'est pas à la dernière position dans le deck
    else
    {
        //On stocke le joker noir dans une variable temporaire
        Card tempJoker = *blackJokerPos;
        //On supprime le joker noir de sa position actuelle
        std::list<Card>::iterator pos = this->m_cardsDeck->erase(blackJokerPos);
        //et on l'insère après la carte qui le suivait dans le deck
        //cela revient à échanger la position du joker noir avec la carte qui le suit directement
        std::advance(pos, 1);
        this->m_cardsDeck->insert(pos, tempJoker);
    }

    std::cout << "=========== Deck apres la premiere etape ===========" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    this->displayDeck();
}

void Deck::secondStep()
{
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "=== Seconde etape -> On fait reculer le joker rouge de deux places ===" << std::endl;
    

    //On récupère la position du joker rouge
    std::list<Card>::iterator redJokerPos = this->findCardByName("RedJoker");
    //On récupère la dernière carte du deck
    Card lastCard = this->m_cardsDeck->back();
    //Ainsi que l'avant dernière carte du deck
    Card penultimateCard = *(std::prev(std::prev(this->m_cardsDeck->end())));

    //Si le joker rouge est la dernière carte du deck
    if((*redJokerPos).getId() == lastCard.getId())
    {
        //On stocke le joker rouge dans une variable temporaire
        Card tempJoker = *redJokerPos;
        //On supprime le joker rouge de sa position actuelle
        this->m_cardsDeck->erase(redJokerPos);
        //et on l'insère à la position 3 dans le deck
        std::list<Card>::iterator newPos = this->m_cardsDeck->begin();
        std::advance(newPos, 2);
        this->m_cardsDeck->insert(newPos, tempJoker);
    }
    //Sinon si le joker rouge est l'avant dernière carte du deck
    else if((*redJokerPos).getId() == penultimateCard.getId())
    {
        //On stocke le joker rouge dans une variable temporaire
        Card tempJoker = *redJokerPos;
        //On supprime le joker rouge de sa position actuelle
        this->m_cardsDeck->erase(redJokerPos);
        //et on l'insère à la position 2 dans le deck
        std::list<Card>::iterator newPos = this->m_cardsDeck->begin();
        std::advance(newPos, 1);
        this->m_cardsDeck->insert(newPos, tempJoker);
    }
    //Sinon le joker rouge n'est ni à l'avant dernière ni à la dernière position dans le deck
    else
    {
        //On stocke le joker rouge dans une variable temporaire
        Card tempJoker = *redJokerPos;
        //On supprime le joker rouge de sa position actuelle
        std::list<Card>::iterator pos = this->m_cardsDeck->erase(redJokerPos);
        //et on l'insère deux places plus loin dans le deck
        std::advance(pos, 2);
        this->m_cardsDeck->insert(pos, tempJoker);
    }

    std::cout << "=========== Deck apres la seconde etape ===========" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    this->displayDeck();
}

void Deck::thirdStep()
{
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "=== Troisieme etape -> On fait une double coupe par rapport aux jokers ===" << std::endl;

    //On récupère la position des deux jokers
    std::list<Card>::iterator blackJokerPos = this->findCardByName("BlackJoker");
    std::list<Card>::iterator redJokerPos = this->findCardByName("RedJoker");

    //On calcule la distance relative entre les deux jokers pour savoir où sont placer les deux deux jokers
    //l'un par rapport à l'autre et donc savoir comment couper dans le paquet de cartes
    int distance = std::distance(this->m_cardsDeck->begin(), blackJokerPos) - std::distance(this->m_cardsDeck->begin(), redJokerPos);
    std::cout << "distance entre les deux jokers: " << distance << std::endl;
    //Si la distance est positive, le joker rouge est situé avant le joker noir
    if(distance > 0)
    {
        //On récupère le second fragment situé entre le joker noir non compris et la fin du deck
        std::advance(blackJokerPos, 1);
        std::list<Card> secondFragment;
        secondFragment.splice(secondFragment.begin(), *this->m_cardsDeck, blackJokerPos, this->m_cardsDeck->end());
        //On récupère le premier fragment situé entre le début du deck et le joker rouge non compris
        std::list<Card> firstFragment;
        firstFragment.splice(firstFragment.begin(), *this->m_cardsDeck, this->m_cardsDeck->begin(), redJokerPos);    
        this->displayDeck();
        //Puis on insère le second fragment à la place du premier et le premier à la place du second dans le deck
        this->m_cardsDeck->insert(this->m_cardsDeck->begin(), secondFragment.begin(), secondFragment.end());
        this->m_cardsDeck->insert(this->m_cardsDeck->end(), firstFragment.begin(), firstFragment.end());
    }
    //Sinon c'est l'inverse (le joker noir est situé avant le joker rouge dans le deck)
    else
    {
        //On récupère le second fragment situé entre le joker rouge non compris et la fin du deck
        std::advance(redJokerPos, 1);
        std::list<Card> secondFragment;
        secondFragment.splice(secondFragment.begin(), *this->m_cardsDeck, redJokerPos, this->m_cardsDeck->end());
        //On récupère le premier fragment situé entre le début du deck et le joker noir non compris
        std::list<Card> firstFragment;
        firstFragment.splice(firstFragment.begin(), *this->m_cardsDeck, this->m_cardsDeck->begin(), blackJokerPos);    
        this->displayDeck();
        //Puis on insère le second fragment à la place du premier et le premier à la place du second dans le deck
        this->m_cardsDeck->insert(this->m_cardsDeck->begin(), secondFragment.begin(), secondFragment.end());
        this->m_cardsDeck->insert(this->m_cardsDeck->end(), firstFragment.begin(), firstFragment.end());
    }

    std::cout << "=========== Deck apres la troisieme etape ===========" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    this->displayDeck();
}

void Deck::fourthStep()
{
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "=== Quatrieme etape -> Coupe simple determinee par la derniere carte ===" << std::endl;

    //On récupère le numéro de la dernière carte
    int n = this->m_cardsDeck->back().getId();
    std::cout << "numero de la derniere carte: " << n << std::endl;
    //On regarde si la dernière carte n'est pas un joker (numéro de carte: 53).
    //Si c'est le cas, la coupe n'est pas nécessaire car cela ne change pas la configuration du deck
    if(n != 53)
    {
        //On récupère les n premières cartes du deck
        //On crée un itérateur qui va pointer sur la carte à l'indice n dans la liste
        std::list<Card>::iterator it = this->m_cardsDeck->begin();
        std::advance(it, n);
        std::list<Card> fragment;
        fragment.splice(fragment.begin(), *this->m_cardsDeck, this->m_cardsDeck->begin(), it);
        //Et on insère ces cartes juste avant la dernière carte du deck
        this->m_cardsDeck->insert(std::prev(this->m_cardsDeck->end()), fragment.begin(), fragment.end());
    }

    std::cout << "=========== Deck apres la quatrieme etape ===========" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    this->displayDeck();
}