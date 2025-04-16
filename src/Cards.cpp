#include "Cards.h"
#include <QRandomGenerator>
#include <algorithm>


void Cards::addCard(const Card& card)
{
	m_cards.insert(card);
}

void Cards::addCard(const Cards& cards)
{
	m_cards.unite(cards.m_cards);//将两个集合或几何对象合并成一个
}

void Cards::addCard(const QVector<Cards>& cards)
{
}

Cards& Cards::operator<<(const Card& card)
{
	addCard(card);
	return *this;
}

Cards& Cards::operator<<(const Cards& cards)
{
	addCard(cards);
	return *this;
}

void Cards::delCard(const Card& card)
{
	m_cards.remove(card);
}

void Cards::delCard(const Cards& cards)
{
	m_cards.subtract(cards.m_cards);
}

void Cards::delCard(const QVector<Cards>& cards)
{
	for (auto it : cards) {
		delCard(it);
	}
}

int Cards::cardCount()
{
	return m_cards.size();
}

bool Cards::isEmpty()
{
	return m_cards.isEmpty();
}

void Cards::clear()
{
	m_cards.clear();
}

Card::CardPoint Cards::maxPoint()
{
	Card::CardPoint max_point = Card::Card_Begin; // 初始化为最小值
	// 遍历 QSet 中的每张牌
	for (auto& card : m_cards) {
		// 获取当前牌的点数
		Card::CardPoint current_point = card.getPoint();
		max_point = std::max(current_point, max_point);
	}
	return max_point; // 返回找到的最大点数
}

Card::CardPoint Cards::minPoint()
{
	Card::CardPoint min_point = Card::CardPoint::Card_End; // 初始化为最大值
	// 遍历 QSet 中的每张牌
	for (auto& card : m_cards) {
		// 获取当前牌的点数
		Card::CardPoint current_point = card.getPoint();
		min_point = std::min(current_point, min_point);
	}
	return min_point; // 返回找到的最小点数
}

int Cards::pointCount(Card::CardPoint point)
{
	int count = 0;
	for (const auto& card : m_cards) {
		// 获取当前牌的点数
		if (point == static_cast<int>(card.getPoint()))
			++count;
	}
	return count;
}

bool Cards::contains(const Card& card)
{
	return m_cards.contains(card);
}

bool Cards::contains(const Cards& cards)
{
	return m_cards.contains(cards.m_cards);
}

Card Cards::takeRandCard()
{
	//生成一个随机数
	int randNum = QRandomGenerator::global()->bounded(m_cards.size());//左闭右开
	QSet<Card>::const_iterator it = m_cards.constBegin();
	//for (int i = 0; i < randNum; ++i,++it);
	// 高级循环，找到要移除的元素
	std::advance(it, randNum); // 将迭代器向前移动 randNum 步
	Card card = *it;
	m_cards.erase(it);
	return card;
}

CardLsit Cards::toCardList(Cards::SortType type)
{
	CardLsit res;
	for (auto& card : m_cards) {
		//res << card;
		res.append(card); // 使用 append，性能更好
	}
	if (type == Cards::SortType::ASC) {
		std::sort(res.begin(), res.end(), lessSort);
	}
	else if (type == Cards::SortType::DESC) {
		std::sort(res.begin(), res.end(), greatorSort);
	}
	return res;
}

Cards::Cards()
{
}

Cards::Cards(const Card& card)
{
	addCard(card);
}

Cards::~Cards()
{
}
