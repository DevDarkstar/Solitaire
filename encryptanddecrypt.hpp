#ifndef ENCRYPTANDDECRYPT_HPP
#define ENCRYPTANDDECRYPT_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <regex>

class EncryptAndDecrypt
{
    private:
    std::string m_codingKeyString;
    std::list<int> *m_codingKeyNumbers;

    public:
    explicit EncryptAndDecrypt(const std::string& key);
    EncryptAndDecrypt();
    ~EncryptAndDecrypt();
    std::string getCodingKey() const;
    std::string convertKeyToString(std::list<int> *key);
    std::list<int>* convertKeyToNumbers(const std::string& key);
    static std::string correctMessage(std::string& message);
    std::string encryptMessage(std::string& message);
    std::string decryptMessage(const std::string& message);
};

#endif