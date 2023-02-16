#ifndef CARD_HPP
#define CARD_HPP

#include <string>

class Card
{
    private:
        int m_id;
        std::string m_name;
    public:
        Card();
        Card(int id, std::string colour);
        ~Card();
        std::string getName();
        int getId();
        void setId(int id);
};

#endif