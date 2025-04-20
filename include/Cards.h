#pragma once

#include <QObject>
#include <QSet>//相当于unordered_set
#include "Card.h"
	
class Cards
{
public:
	enum SortType { ASC, DESC, NOSORT };
	//添加扑克牌
	void addCard(const Card& card);
	void addCard(const Cards& cards);
	void addCard(const QVector<Cards>& cards);

	//一次性插入多个数据 （操作符重载<<）
	Cards& operator <<(const Card& card);
	Cards& operator <<(const Cards& cards);

	//删除扑克牌
	void delCard(const Card& card);
	void delCard(const Cards& cards);
	void delCard(const QVector<Cards>& cards);

	//扑克牌的数量
	int cardCount();
	//是否为空
	bool isEmpty();
	//清空扑克牌
	void clear();

	//最大点数
	Card::CardPoint maxPoint();
	//最小点数
	Card::CardPoint minPoint();
	//指定点数的牌的数量
	int pointCount(Card::CardPoint point);
	//某张牌是否在集合中
	bool contains(const Card& card);
	bool contains(const Cards& cards);

	//随机取出一张牌
	Card takeRandCard();

	//排序
	CardLsit toCardList(Cards::SortType type = DESC);

	//测试，输出cards的卡牌点数
	void getAllCardsPoint();

public:
	Cards();
	explicit Cards(const Card& card);
	~Cards();
private:
	QSet<Card>m_cards;//QSet容器中为自定义类时，需要重写==和qHash函数，见官方文档
};


