#include "Card.h"

Card::Card()
{

}

Card::~Card()
{

}

void Card::setPoint(CardPoint point)
{
	m_point = point;
}

void Card::setSuit(CardSuit suit)
{
	m_suit = suit;
}

Card::CardPoint Card::getPoint() const
{
	return m_point;
}

Card::CardSuit Card::getSuit() const
{
	return m_suit;
}
