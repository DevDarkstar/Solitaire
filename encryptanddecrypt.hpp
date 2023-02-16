#ifndef ENCRYPTANDDECRYPT_HPP
#define ENCRYPTANDDECRYPT_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>

class EncryptAndDecrypt
{
    private:
    std::string m_codingKeyString;
    std::list<int> *m_codingKeyNumbers;

    public:
    EncryptAndDecrypt(const std::string& key);
    EncryptAndDecrypt();
    ~EncryptAndDecrypt();
    std::string getCodingKey();
    std::string convertKeyToString(std::list<int> *key);
    std::list<int>* convertKeyToNumbers(const std::string& key);
    std::string encryptMessage(std::string& message);
    std::string decryptMessage(const std::string& message);
};

#endif