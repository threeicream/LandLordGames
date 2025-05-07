#include "Card.h"
#include <QtCore/qhash.h> // 包含 qHash 函数的声明

uint qHash(const Card& card, uint seed)
{
	uint hash = seed;
	hash = qHash(static_cast<int>(card.getPoint()), static_cast<size_t>(hash));
	hash = qHash(static_cast<int>(card.getSuit()), static_cast<size_t>(hash));
	return hash;
}

Card::Card()
	:m_point(),m_suit()
{

}

Card::Card(const CardPoint point, const CardSuit suit)
{
	setPoint(point);
	setSuit(suit);
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

bool Card::operator==(const Card& other) const
{
	return (m_point == other.m_point) && (m_suit == other.m_suit);
}

bool Card::operator<(const Card& other) const
{
	if (this->getPoint() == other.getPoint()) {
		return this->getSuit() < other.getSuit();
	}
	else
		return this->getPoint() < other.getPoint();
}

bool lessSort(const Card& a, const Card& b)
{
	if (a.getPoint() == b.getPoint()) {
		return a.getSuit() < b.getSuit();
	}
	else
		return a.getPoint() < b.getPoint();
}

bool greatorSort(const Card& a, const Card& b)
{
	if (a.getPoint() == b.getPoint()) {
		return a.getSuit() > b.getSuit();
	}
	else
		return a.getPoint() > b.getPoint();
}