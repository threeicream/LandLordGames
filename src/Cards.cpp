#include "Cards.h"
#include <QRandomGenerator>
#include <algorithm>
#include <QDebug>

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

//Card Cards::takeRandCard()
//{
//	//生成一个随机数
//	int randNum = QRandomGenerator::global()->bounded(m_cards.size());//左闭右开
//	QSet<Card>::const_iterator it = m_cards.constBegin();
//	Card card;  // 用于存储最终要返回的牌
//
//	if (!(xy % 3) && tt < 6) {
//		Card::CardPoint point = static_cast<Card::CardPoint>(tt / 3 + 3);//指定点数为5，6
//		Card::CardSuit suit = static_cast<Card::CardSuit>(tt % 4 + 1);//指定花色
//		Card targetCard(point, suit);
//		it = m_cards.find(targetCard);
//
//		if (it != m_cards.constEnd()) {
//			// 找到了指定的牌
//			card = *it;
//			qDebug() << "本次发牌点数：指定牌" << card.getPoint() + 2 << QString(" 第%1次发牌").arg(xy);
//			m_cards.erase(it  );
//			xy++;
//			tt++; // 放在这里确保每次尝试指定牌都递增 tt
//			return card;
//		}
//		else {
//			// 没有找到指定的牌，回退到随机发牌
//			qDebug() << "未找到指定牌，回退到随机发牌";
//		}
//	}
//	// 随机发牌
//	else if (tt < 6) { //如果 tt<6 仍然有指定发牌的需求，但本次不满足指定发牌条件
//		it = m_cards.constBegin(); // 每次都从头开始
//		std::advance(it, randNum);
//
//		// 避免死循环，限制尝试次数
//		int attempts = 0;
//		while (attempts < 10 && it != m_cards.constEnd() && (*it).getPoint() == static_cast<Card::CardPoint>(tt / 3 + 3)) {
//			randNum = QRandomGenerator::global()->bounded(m_cards.size());//重新生成随机数
//			it = m_cards.constBegin(); // 重新从头开始
//			std::advance(it, randNum);
//			attempts++;
//		}
//
//		if (it != m_cards.constEnd()) {
//			card = *it;
//			qDebug() << "本次发牌点数：随机牌(但尝试避免指定牌)" << card.getPoint() + 2 << QString(" 第%1次发牌").arg(xy);
//			m_cards.erase(it);
//			xy++;
//			//tt++; // 即使是随机发牌，也要递增 tt，保证它最终会超过 6
//			return card;
//		}
//		else {
//			qDebug() << "警告：无法找到合适的随机牌(避免指定牌)!";
//			//tt++; // 即使找不到合适的牌，也要递增 tt，保证它最终会超过 6
//			return Card(); // 返回默认牌，表示发牌失败。 你的 Card 类需要有一个默认构造函数。
//		}
//	}
//	else { // tt >= 6, 完全随机发牌
//		it = m_cards.constBegin();
//		std::advance(it, randNum); // 将迭代器向前移动 randNum 步
//		if (it != m_cards.constEnd()) {
//			card = *it;
//			qDebug() << "本次发牌点数：完全随机牌" << card.getPoint() + 2 << QString(" 第%1次发牌").arg(xy);
//			m_cards.erase(it);
//			xy++;
//			return card;
//		}
//		else {
//			qDebug() << "警告：无法找到任何牌! 牌堆可能已经空了";
//			return Card(); // 返回默认牌
//		}
//	}
//}

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

void Cards::getAllCardsPoint()
{
	CardLsit res = toCardList();
	for (auto& it : res) {
		qDebug() << it.getPoint();
	}
	qDebug() <<" ";
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
