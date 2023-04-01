#include "deck.hpp"
#include "encryptanddecrypt.hpp"
#include "solitairetest.hpp"
#include <fstream>

int main()
{
    //Création de la classe de test pour tester la robustesse du programme
    SolitaireTest* test = new SolitaireTest();
    test->tests();

    delete test;
    //Création du deck du créateur des messages cryptés
    Deck d_creator;
    d_creator.shuffleDeck("1");

    //Création ensuite du deck du correspondant contenant le même deck que le créateur des messages
    Deck d_correspondant(d_creator.getDeck());

    //On déclare un booléen qui gère le fonctionnement du programme
    bool isRunning = true;
    //Et une liste de noms de fichier contenant les messages cryptés créés pendant l'exécution du programme
    std::list<std::string> fileNames;

    do{
        char answer[20];

        std::cout << "\n********************************\n";
        std::cout << "Que souhaitez-vous faire ?\n";
        std::cout << "1) -- Crypter un message --\n";
        std::cout << "2) -- Decrypter un message --\n";
        std::cout << "3) -- Resynchroniser les deux decks --\n";
        std::cout << "4) -- Quitter le programme --\n";
        std::cout << "********************************"<< std::endl;

        std::cin >> answer;
        int displayInfos = atoi(answer);

        switch(displayInfos)
        {
            case 1:
            {
                //synchronisation de std::cin en fonction du système d'exploitation
                #if defined(_WIN32)
                    std::cin.clear();
                    std::cin.sync();
                #elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                #endif              
                std::string message;
                std::cout << "Indiquer le message que vous souhaitez crypter: ";
                std::getline(std::cin, message);

                #if defined(_WIN32)
                    std::cin.clear();
                    std::cin.sync();
                #endif  

                std::string fileName;
                std::cout << "Indiquer le nom du fichier dans lequel sauvegarder votre message: ";
                std::getline(std::cin, fileName);

                std::string displayInfos;
                do{
                    #if defined(_WIN32)
                        std::cin.clear();
                        std::cin.sync();
                    #endif
                    std::cout << "Voulez-vous afficher les informations detaillees du programme?\n";
                    std::cout << "'O' pour oui - 'N' pour non: ";
                    std::getline(std::cin, displayInfos);

                    if(displayInfos.compare("O") == 0)
                    {
                        Deck::displayInfos = true;
                        EncryptAndDecrypt::displayInfos = true;
                    }
                    else if(displayInfos.compare("N") == 0)
                    {
                        Deck::displayInfos = false;
                        EncryptAndDecrypt::displayInfos = false;
                    }
                    else
                        std::cout << "La valeur indiquee est incorrect...\n";
                }while(displayInfos.compare("O") != 0 && displayInfos.compare("N") != 0);               

                //Création du fichier en mode écriture
                std::ofstream file(fileName);
                if(file)
                {
                    //On transforme le message (suppression des accents, des caractères de fin de phrase,...) afin de faciliter son cryptage
                    std::string correctedMessage = "";
                    #if defined(_WIN32)
                        correctedMessage = EncryptAndDecrypt::correctMessage(message);
                    #elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
                        correctedMessage = EncryptAndDecrypt::correctMessageUnicode(message);
                    #endif 
                    //On génère une clé de cryptage de la même taille que le message à crypter
                    std::string keyStream = d_creator.createKeyStream(correctedMessage.size());
                    EncryptAndDecrypt cad(keyStream);
                    //On crypte le message
                    std::string encryptedMessage = cad.encryptMessage(correctedMessage);
                    //Et on le stocke dans le fichier
                    file << encryptedMessage;
                    file.close();
                    fileNames.push_back(fileName);
                }
                else
                {
                    std::cout << "\n+++ Impossible d'ouvrir le fichier demande... +++" << std::endl;
                    isRunning = false;
                }
                break;
            }
            case 2:
            {
                #if defined(_WIN32)
                    std::cin.clear();
                    std::cin.sync();
                #elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                #endif  
                std::string fileName;
                std::cout << "Indiquez le nom de fichier contenant le nom de fichier que vous souhaitez decrypter: ";
                std::getline(std::cin, fileName);

                std::string displayInfos;
                do{
                    #if defined(_WIN32)
                        std::cin.clear();
                        std::cin.sync();
                    #endif
                    std::cout << "Voulez-vous afficher les informations detaillees du programme?\n";
                    std::cout << "'O' pour oui - 'N' pour non: ";
                    std::getline(std::cin, displayInfos);

                    if(displayInfos.compare("O") == 0)
                    {
                        Deck::displayInfos = true;
                        EncryptAndDecrypt::displayInfos = true;
                    }
                    else if(displayInfos.compare("N") == 0)
                    {
                        Deck::displayInfos = false;
                        EncryptAndDecrypt::displayInfos = false;
                    }
                    else
                        std::cout << "La valeur indiquee est incorrect...\n";
                }while(displayInfos.compare("O") != 0 && displayInfos.compare("N") != 0);

                std::ifstream file(fileName);
                if(file)
                {
                    std::string message;
                    file >> message;

                    //On génère une clé de décryptage à partir de la taille du message crypté
                    std::string keyStream_correspondant = d_correspondant.createKeyStream(message.size());
                    EncryptAndDecrypt cad_correspondant(keyStream_correspondant);

                    //Puis on le décrypte pour savoir si le fonctionnement est bon
                    std::string finalMessage = cad_correspondant.decryptMessage(message);
                    std::cout << "\n*************************************************\n";
                    std::cout << "Le message decrypte est : " << finalMessage << "\n";
                    std::cout << "*************************************************" << std::endl;
                    file.close();
                }
                else
                    std::cout << "\n+++ Le fichier que vous souhaitez ouvrir n'existe pas... +++" << std::endl;
                break;
            }
            case 3:
            {
                std::cout << "Resynchronisation des deux decks...\n";
                d_correspondant.setDeck(d_creator.getDeck());
                //On supprime tous les fichiers créés jusqu'à là car ils ne peuvent plus être lus
                for(std::list<std::string>::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
                {
                    std::remove((*it).c_str());
                }
                //Et on vide la liste
                fileNames.clear();
                std::cout << "\n+++ Les deux decks ont ete resynchronises. +++" << std::endl;
                break;
            }
            case 4:
            {
                std::cout << "\nSortie du programme..." << std::endl;
                //On supprime tous les fichiers créés
                for(std::list<std::string>::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
                {
                    std::remove((*it).c_str());
                }
                isRunning = false;
                break;
            }
            default:
                std::cout << "\n+++ Le choix que vous avez indique est incorrect... +++" << std::endl;
        }
    }while(isRunning);

    return 0;
}
