#include "encryptanddecrypt.hpp"

EncryptAndDecrypt::EncryptAndDecrypt(const std::string& key)
{
    this->m_codingKeyString = key;
    this->m_codingKeyNumbers = nullptr;
    this->m_codingKeyNumbers = this->convertKeyToNumbers(key);
    std::cout << "La cle vaut " << this->m_codingKeyString << " et sa representation sous forme numerique est ";
    for(std::list<int>::iterator it = this->m_codingKeyNumbers->begin(); it != this->m_codingKeyNumbers->end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

EncryptAndDecrypt::EncryptAndDecrypt()
{
    this->m_codingKeyString = "";
    this->m_codingKeyNumbers = nullptr;
    this->m_codingKeyNumbers = new std::list<int>();
}

EncryptAndDecrypt::~EncryptAndDecrypt()
{
    this->m_codingKeyNumbers->clear();
    this->m_codingKeyNumbers = nullptr;
}

std::string EncryptAndDecrypt::getCodingKey()
{
    return m_codingKeyString;
}

std::string EncryptAndDecrypt::convertKeyToString(std::list<int> *key)
{
    //On crée la chaine de caractères qui contiendra le résultat un fois converti
    std::string convertedKey = "";
    //ainsi qu'une autre chaine contenant l'alphabet
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //On convertit la liste de nombres en chaine de caractères
    for(std::list<int>::iterator it = key->begin(); it != key->end(); ++it)
    {
        if(*it == 0)
            convertedKey += ' ';
        else
            convertedKey += alphabet[(*it) - 1];
    }

    return convertedKey;
}

std::list<int> *EncryptAndDecrypt::convertKeyToNumbers(const std::string& key)
{
    //On initialise la liste d'entiers qui contiendra la clé une fois convertie en nombres
    std::list<int> *convertedKey = new std::list<int>();
    //ainsi qu'une chaine de caractères contenant l'alphabet
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //On convertit la chaine de caractères en liste de nombres
    for(int i = 0; i < key.size(); i++)
    {
        if(key[i] == ' ')
            convertedKey->push_back(0);
        else
            convertedKey->push_back(std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), key[i])) + 1);
    }

    return convertedKey;
}

std::string EncryptAndDecrypt::encryptMessage(std::string& message)
{
    //On passe l'intégralité du message en majuscules
    std::transform(message.begin(), message.end(), message.begin(), [](char c)
    {
        return std::toupper(c);
    });

    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "========== Cryptage du message " << message << " ==========" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "========== Cle utilisee pour le cryptage: " << this->getCodingKey() << " ==========" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;

    //On transforme le message à crypter d'une chaine de caractères à une liste de nombres
    std::list<int> *messageToNumbers = nullptr;
    messageToNumbers = this->convertKeyToNumbers(message);

    //Pour créer le message crypté, on additionne terme à terme les deux listes de nombres (celle de la clé de cryptage
    //avec celle du message à crypter) que l'on stocke dans la liste de nombres du message crypté
    std::list<int> *encryptedMessage = nullptr;
    encryptedMessage = new std::list<int>();
    for(std::list<int>::iterator it1 = this->m_codingKeyNumbers->begin(), it2 = messageToNumbers->begin(); it1 != this->m_codingKeyNumbers->end() && it2 != messageToNumbers->end(); ++it1, ++it2)
    {
        int value = *it1 + *it2;
        //Si la valeur obtenue dépasse 26, on y soustrait 26
        if(value > 26)
            value -= 26;
        //Puis on ajoute cette valeur dans la liste de nombres de message crypté
        encryptedMessage->push_back(value);
    }

    //On vide la liste contenant le message à crypter
    messageToNumbers->clear();
    messageToNumbers = nullptr;

    std::cout << "========== Message crypte sous forme numerique: ";
    for(std::list<int>::iterator it = encryptedMessage->begin(); it != encryptedMessage->end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;

    //Enfin, on transforme le message crypté en chaine de caractères
    std::string finalMessage = this->convertKeyToString(encryptedMessage);

    //On vide la liste contenant le message crypté et on retourne le message final crypté
    encryptedMessage->clear();
    encryptedMessage = nullptr;

    std::cout << "========== Le message crypte est: " << finalMessage << " ==========" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;

    return finalMessage;
}

std::string EncryptAndDecrypt::decryptMessage(const std::string& message)
{
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "========== Decryptage du message " << message << " ==========" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "========== Cle utilisee pour le decryptage: " << this->getCodingKey() << " ==========" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;

    //On transforme le message à décrypter d'une chaine de caractères à une liste de nombres
    std::list<int> *messageToNumbers = nullptr;
    messageToNumbers = this->convertKeyToNumbers(message);

    //Pour créer le message décrypté, on soustrait terme à terme les nombres du message à décrypter
    //avec ceux de la liste de la clé de cryptage que l'on stocke dans la liste de nombres du message décrypté
    std::list<int> *decryptedMessage = nullptr;
    decryptedMessage = new std::list<int>();
    for(std::list<int>::iterator it1 = this->m_codingKeyNumbers->begin(), it2 = messageToNumbers->begin(); it1 != this->m_codingKeyNumbers->end() && it2 != messageToNumbers->end(); ++it1, ++it2)
    {
        int value = *it2 - *it1;
        //Si la valeur obtenue est inférieure à 0, on y ajoute 26
        if(value < 0)
            value += 26;
        //Puis on ajoute cette valeur dans la liste de nombres de message décrypté
        decryptedMessage->push_back(value);
    }

    //On vide la liste contenant le message à crypter
    messageToNumbers->clear();
    messageToNumbers = nullptr;

    std::cout << "========== Message decrypte sous forme numerique: ";
    for(std::list<int>::iterator it = decryptedMessage->begin(); it != decryptedMessage->end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;

    //Enfin, on transforme le message décrypté en chaine de caractères
    std::string finalMessage = this->convertKeyToString(decryptedMessage);

    //On vide la liste contenant le message décrypté et on retourne le message final décrypté
    decryptedMessage->clear();
    decryptedMessage = nullptr;

    std::cout << "========== Le message decrypte est: " << finalMessage << " ==========" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;

    return finalMessage;
}

