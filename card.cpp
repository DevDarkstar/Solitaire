#include "card.hpp"

Card::Card()
{
    this->m_id = 0;
    this->m_name = "";
}

Card::Card(int id, std::string name)
{
    this->m_id = id;
    this->m_name = name;
}

Card::~Card(){}

std::string Card::getName()
{
    return m_name;
}

int Card::getId()
{
    return m_id;
}

void Card::setId(int id)
{
    this->m_id = id;
}

