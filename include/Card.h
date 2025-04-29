#pragma once
#include <QVector>
//#include <QMetaType>
//#include <iterator>
class Card;

using CardLsit = QVector<Card>;

class Card
{
public:
	//花色
	enum CardSuit { Suit_Begin, DIAMOD, CLUB, HEART, SPADE, Suit_End };
	//点数
	enum CardPoint{
		Card_Begin,
		Card_3,
		Card_4,
		Card_5,
		Card_6,
		Card_7,
		Card_8,
		Card_9,
		Card_10,
		Card_J,
		Card_Q,
		Card_K,
		Card_A,
		Card_2,
		Card_SJ,//small joker
		Card_BJ,//big joker
		Card_End
	};
	
	Card();
	explicit Card(const CardPoint point, const CardSuit suit);
	~Card();
public:
	void setPoint(CardPoint point);
	void setSuit(CardSuit suit);
	CardPoint getPoint() const;
	CardSuit getSuit() const;

	// 重载 == 运算符
	bool operator==(const Card& other) const;
	//重写全局函数qHash

	//重载 < 运算符
	bool operator<(const Card& other) const;
private:
	CardPoint m_point;
	CardSuit m_suit;
};

//对象比较
bool lessSort(const Card& a, const Card& b);
bool greatorSort(const Card& a, const Card& b);

uint qHash(const Card& card, uint seed = 0);//设置一个哈希键值

//// 注册 CardPoint 和 CardSuit 到 Qt 的元对象系统
//Q_DECLARE_METATYPE(Card::CardPoint)
//Q_DECLARE_METATYPE(Card::CardSuit)