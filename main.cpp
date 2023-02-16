#include "deck.hpp"
#include "encryptanddecrypt.hpp"
#include <ctime>
#include <cstdlib>

int main()
{
    //génération des nombres pseudo-aléatoires
    srand((unsigned)time(NULL));
    //On crée un message à crypter puis à décrypter
    std::string message = "Je cherche la reponse";
    //On récupère la taille du message
    int messageSize = message.size();
    Deck d;
    d.displayDetailedDeck();
    std::cout << "melange du deck...\n" << std::endl;
    d.shuffleDeck();
    //On génère une clé de cryptage de la même taille que le message à crypter
    std::string keyStream = d.createKeyStream(messageSize);

    EncryptAndDecrypt cad(keyStream);
    //On crypte le message
    std::string encryptedMessage = cad.encryptMessage(message);

    //Puis on le décrypte pour savoir si le fonctionnement est bon
    std::string finalMessage = cad.decryptMessage(encryptedMessage);

    return 0;
}

