#include "encryptanddecrypt.hpp"

//Permet d'afficher des informations détaillées sur le déroulement du programme
bool EncryptAndDecrypt::displayInfos = false;

EncryptAndDecrypt::EncryptAndDecrypt(): m_codingKeyString(""), m_codingKeyNumbers(new std::list<int>())
{

}

EncryptAndDecrypt::EncryptAndDecrypt(const std::string& key): m_codingKeyString(key), m_codingKeyNumbers(this->convertKeyToNumbers(key))
{
    if(EncryptAndDecrypt::displayInfos)
    {
        std::cout << "La cle vaut " << this->m_codingKeyString << " et sa representation sous forme numerique est ";
        for(std::list<int>::iterator it = this->m_codingKeyNumbers->begin(); it != this->m_codingKeyNumbers->end(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
}

EncryptAndDecrypt::~EncryptAndDecrypt()
{
    this->m_codingKeyNumbers->clear();
    delete this->m_codingKeyNumbers;
}

void EncryptAndDecrypt::setCodingKeyString(const std::string& key)
{
    this->m_codingKeyString = key;
    this->setCodingKeyNumbers(this->convertKeyToNumbers(key));
}

void EncryptAndDecrypt::setCodingKeyNumbers(std::list<int> *key)
{
    this->m_codingKeyNumbers->clear();
    for(std::list<int>::iterator it = key->begin(); it != key->end(); ++it)
    {
        this->m_codingKeyNumbers->push_back(*it);
    }
}

std::string EncryptAndDecrypt::getCodingKey() const
{
    return m_codingKeyString;
}

std::list<int>* EncryptAndDecrypt::getCodingKeyNumbers() const
{
    return m_codingKeyNumbers;
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
        convertedKey += alphabet[*it - 1];
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
        convertedKey->push_back(std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), key[i])) + 1);
    }

    return convertedKey;
}

std::string& EncryptAndDecrypt::addDotsAndSpaces(std::string& message)
{
    //On passe l'intégralité du message en minuscules sauf la première lettre
    std::string::iterator it = message.begin();
    std::advance(it, 1);
    std::transform(it, message.end(), it, [&](char c)
    {
        return std::tolower(c);
    });

    size_t found = 0;
    //On fait pareil pour les espaces
    do{
        found = message.find("sz", found);
        if(found != std::string::npos)
        {
            message.replace(found, 2, " ");
            found++;
        }
    }while(found != std::string::npos);

    found = 0;
    do{
        found = message.find("stop", found);
        if(found != std::string::npos)
        {
            message.replace(found, 4, ".");
            found += 2;
            //On récupère le caractère suivant
            char c = message[found];
            //Et on le passe en majuscules
            message[found] = c - 32;
        }
    }while(found != std::string::npos);

    return message;
}

std::string EncryptAndDecrypt::correctMessage(std::string& message)
{
    std::string tempMessage;
    //Et on actualise le message en remplaçant les caractères spéciaux par leur version classique
    //pour les a accentués
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("\x85|\x83|\x84|\x8E"), "a");
    message = "";
    //pour les e accentués
    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("\x82|\x8A|\x88|\x89|\x90"), "e");
    tempMessage = "";
    //pour les i accentués
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("\x8B|\x8C|\x8D"), "i");
    message = "";
    //pour les o accentués
    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("\x93|\x94|\x95"), "o");
    tempMessage = "";
    //pour les u accentués
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("\x96|\x97|\x81"), "u");
    message = "";
    //pour le c cédille
    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("\x87|\x80"), "c");
    tempMessage = "";
    //les caractères de milieu de phrase
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex(",|:|\\(|\\)|\\[|\\]|\""), "");
    message = "";
    //pour le e commercial (&)
    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("\x26"), "et");
    tempMessage = "";
    //pour les caractères de fin de phrase
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("\\.|!|;|\\?"), "stop");
    message = "";
    //et enfin pour les caractères de liaison
    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("\\-|'|_| "), "sz");
    tempMessage = "";

    //On fait de même pour les 10 premiers chiffres
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("0"), "zero");
    message = "";

    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("1"), "un");
    tempMessage = "";

    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("2"), "deux");
    message = "";

    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("3"), "trois");
    tempMessage = "";

    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("4"), "quatre");
    message = "";

    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("5"), "cinq");
    tempMessage = "";

    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("6"), "six");
    message = "";

    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("7"), "sept");
    tempMessage = "";

    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("8"), "huit");
    message = "";

    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("9"), "neuf");
    tempMessage = "";

    return message;
}

std::string EncryptAndDecrypt::correctMessageUnicode(std::string& message)
{
    std::string tempMessage;
    //Et on actualise le message en remplaçant les caractères spéciaux par leur version classique
    //pour les a accentués
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("\xc3\xa0|\xc3\xa1|\xc3\xa2|\xc3\x80|\xc3\xa4"), "a");
    message = "";
    //pour les e accentués
    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("\xc3\xa8|\xc3\xa9|\xc3\xaa|\xc3\xab|\xc3\x89"), "e");
    tempMessage = "";
    //pour les i accentués
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("\xc3\x8e|\xc3\xae|\xc3\xaf"), "i");
    message = "";
    //pour les o accentués
    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("\xc3\xb4|\xc3\xb6|\xc3\x94"), "o");
    tempMessage = "";
    //pour les u accentués
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("\xc3\xb9|\xc3\xbb|\xc3\xbc"), "u");
    message = "";
    //pour le c cédille
    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("\xc3\xa7|\xc3\x87"), "c");
    tempMessage = "";
    //les caractères de milieu de phrase
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex(",|:|\\(|\\)|\\[|\\]|\""), "");
    message = "";
    //pour le e commercial (&)
    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("\x26"), "et");
    tempMessage = "";
    //pour les caractères de fin de phrase
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("\\.|!|;|\\?"), "stop");
    message = "";
    //et enfin pour les caractères de liaison
    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("\\-|'|’|_| "), "sz");
    tempMessage = "";

    //On fait de même pour les 10 premiers chiffres
    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("0"), "zero");
    message = "";

    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("1"), "un");
    tempMessage = "";

    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("2"), "deux");
    message = "";

    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("3"), "trois");
    tempMessage = "";

    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("4"), "quatre");
    message = "";

    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("5"), "cinq");
    tempMessage = "";

    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("6"), "six");
    message = "";

    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("7"), "sept");
    tempMessage = "";

    std::regex_replace(std::back_inserter(tempMessage) , message.begin(), message.end(), std::regex("8"), "huit");
    message = "";

    std::regex_replace(std::back_inserter(message) , tempMessage.begin(), tempMessage.end(), std::regex("9"), "neuf");
    tempMessage = "";

    return message;
}

std::string EncryptAndDecrypt::encryptMessage(std::string& message)
{
    //On passe l'intégralité du message en majuscules
    std::transform(message.begin(), message.end(), message.begin(), [&](char c)
    {
        return std::toupper(c);
    });

    if(EncryptAndDecrypt::displayInfos)
    {
        std::cout << "-----------------------------------------------------------------------\n";
        std::cout << "========== Cryptage du message: " << message << " ==========\n";
        std::cout << "-----------------------------------------------------------------------\n";
        std::cout << "========== Cle utilisee pour le cryptage: " << this->getCodingKey() << " ==========\n";
        std::cout << "-----------------------------------------------------------------------" << std::endl;
    }

    //On transforme le message à crypter d'une chaine de caractères à une liste de nombres
    std::list<int> *messageToNumbers = this->convertKeyToNumbers(message);

    //Pour créer le message crypté, on additionne terme à terme les deux listes de nombres (celle de la clé de cryptage
    //avec celle du message à crypter) que l'on stocke dans la liste de nombres du message crypté
    std::list<int> *encryptedMessage = new std::list<int>();
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
    delete messageToNumbers;

    if(EncryptAndDecrypt::displayInfos)
    {
        std::cout << "========== Message crypte sous forme numerique: ";
        for(std::list<int>::iterator it = encryptedMessage->begin(); it != encryptedMessage->end(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << "\n-----------------------------------------------------------------------" << std::endl;
    }

    //Enfin, on transforme le message crypté en chaine de caractères
    std::string finalMessage = this->convertKeyToString(encryptedMessage);

    //On vide la liste contenant le message crypté et on retourne le message final crypté
    encryptedMessage->clear();
    delete encryptedMessage;

    if(EncryptAndDecrypt::displayInfos)
    {
        std::cout << "========== Le message crypte est: " << finalMessage << " ==========\n";
        std::cout << "-----------------------------------------------------------------------" << std::endl;
    }

    return finalMessage;
}

std::string EncryptAndDecrypt::decryptMessage(const std::string& message)
{
    if(EncryptAndDecrypt::displayInfos)
    {
        std::cout << "-----------------------------------------------------------------------\n";
        std::cout << "========== Decryptage du message: " << message << " ==========\n";
        std::cout << "-----------------------------------------------------------------------\n";
        std::cout << "========== Cle utilisee pour le decryptage: " << this->getCodingKey() << " ==========\n";
        std::cout << "-----------------------------------------------------------------------" << std::endl;
    }

    //On transforme le message à décrypter d'une chaine de caractères à une liste de nombres
    std::list<int> *messageToNumbers = this->convertKeyToNumbers(message);

    //Pour créer le message décrypté, on soustrait terme à terme les nombres du message à décrypter
    //avec ceux de la liste de la clé de cryptage que l'on stocke dans la liste de nombres du message décrypté
    std::list<int> *decryptedMessage = new std::list<int>();
    for(std::list<int>::iterator it1 = this->m_codingKeyNumbers->begin(), it2 = messageToNumbers->begin(); it1 != this->m_codingKeyNumbers->end() && it2 != messageToNumbers->end(); ++it1, ++it2)
    {
        int value = *it2 - *it1;
        //Si la valeur obtenue est inférieure à 1, on y ajoute 26
        if(value < 1)
            value += 26;
        //Puis on ajoute cette valeur dans la liste de nombres de message décrypté
        decryptedMessage->push_back(value);
    }

    //On vide la liste contenant le message à crypter
    messageToNumbers->clear();
    delete messageToNumbers;

    if(EncryptAndDecrypt::displayInfos)
    {
        std::cout << "========== Message decrypte sous forme numerique: ";
        for(std::list<int>::iterator it = decryptedMessage->begin(); it != decryptedMessage->end(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << "\n-----------------------------------------------------------------------" << std::endl;
    }

    //Enfin, on transforme le message décrypté en chaine de caractères
    std::string finalMessage = this->convertKeyToString(decryptedMessage);

    //On vide la liste contenant le message décrypté et on retourne le message final décrypté
    decryptedMessage->clear();
    delete decryptedMessage;

    finalMessage = this->addDotsAndSpaces(finalMessage);

    if(EncryptAndDecrypt::displayInfos)
    {
        std::cout << "========== Le message decrypte est: " << finalMessage << " ==========\n";
        std::cout << "-----------------------------------------------------------------------" << std::endl;
    }

    return finalMessage;
}
