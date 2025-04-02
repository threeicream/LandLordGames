#pragma once
#include <QVector>
//using CardLsit = QVector<Card>;

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
	~Card();
public:
	void setPoint(CardPoint point);
	void setSuit(CardSuit suit);
	CardPoint getPoint() const;
	CardSuit getSuit() const;
private:
	CardPoint m_point;
	CardSuit m_suit;
};
