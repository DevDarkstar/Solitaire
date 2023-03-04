#include "deck.hpp"
#include "encryptanddecrypt.hpp"

int main()
{
    //On crée un message à crypter puis à décrypter
    std::string message = "Celä ne nous rëgarde, pas.";
    //On transforme le message (suppression des accents, des caractères de fin de phrase,...) afin de faciliter son cryptage
    std::string correctedMessage = EncryptAndDecrypt::correctMessage(message); 
    //On récupère la taille du message
    int messageSize = correctedMessage.size();
    //Création du deck du créateur des messages crypés
    Deck d_creator;
    d_creator.displayDetailedDeck();
    std::cout << "melange du deck...\n" << std::endl;
    d_creator.shuffleDeck();

    //Création ensuite du deck du correspondant contenant le même deck que le créateur des messages
    Deck d_correspondant(d_creator.getDeck());
    //On génère une clé de cryptage de la même taille que le message à crypter
    std::string keyStream = d_creator.createKeyStream(messageSize);

    EncryptAndDecrypt cad(keyStream);
    //On crypte le message
    std::string encryptedMessage = cad.encryptMessage(correctedMessage);

    //On l'envoie au correspondant
    //On génère une clé de décryptage à partir de la taille du message crypté
    std::string keyStream_correspondant = d_correspondant.createKeyStream(encryptedMessage.size());

    EncryptAndDecrypt cad_correspondant(keyStream_correspondant);

    //Puis on le décrypte pour savoir si le fonctionnement est bon
    std::string finalMessage = cad_correspondant.decryptMessage(encryptedMessage);

    return 0;
}

