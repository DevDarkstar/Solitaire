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
    EncryptAndDecrypt();
    explicit EncryptAndDecrypt(const std::string& key);
    ~EncryptAndDecrypt();
    void setCodingKeyString(const std::string& key);
    void setCodingKeyNumbers(std::list<int> *key);
    std::string getCodingKey() const;
    std::list<int>* getCodingKeyNumbers() const;
    std::string convertKeyToString(std::list<int> *key);
    std::list<int>* convertKeyToNumbers(const std::string& key);
    std::string& replaceStopByDots(std::string& message);
    static std::string correctMessage(std::string& message);
    std::string encryptMessage(std::string& message);
    std::string decryptMessage(const std::string& message);
};

#endif