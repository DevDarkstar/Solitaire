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
        std::string getName() const;
        int getId() const;
        void setId(int id);
        friend bool operator==(const Card& card1, const Card& card2);
};

#endif