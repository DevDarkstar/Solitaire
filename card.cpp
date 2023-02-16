#include "card.hpp"

Card::Card(): m_id(0), m_name("")
{

}

Card::Card(int id, std::string name): m_id(id), m_name(name)
{

}

Card::~Card()
{

}

std::string Card::getName() const
{
    return m_name;
}

int Card::getId() const
{
    return m_id;
}

void Card::setId(int id)
{
    this->m_id = id;
}

