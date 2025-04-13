#include "PlayHand.h"

void PlayHand::classify(Cards& cards)
{
	QVector<Card> list = cards.toCardList(Cards::ASC);
	int cardRecord[Card::Card_End];
	memset(cardRecord, 0, sizeof(int) * Card::Card_End);//将内存区域设置为 0
	for (int i = 0; i < list.size(); ++i) {//相当于计数排序的方法
		Card c = list[i];
		++cardRecord[c.getPoint()];
	}
	m_oneCard.clear();
	m_twoCard.clear();
	m_threeCard.clear();
	m_fourCard.clear();
	for (int i = 0; i < Card::Card_End; ++i) {
		switch (cardRecord[i])
		{
		case 1:
			m_oneCard.append(static_cast<Card::CardPoint>(i));
			break;
		case 2:
			m_twoCard.append(static_cast<Card::CardPoint>(i));
			break;
		case 3:
			m_threeCard.append(static_cast<Card::CardPoint>(i));
			break;
		case 4:
			m_fourCard.append(static_cast<Card::CardPoint>(i));
			break;
		default:
			break;
		}
	}
}

void PlayHand::judgeCardType()
{
}

PlayHand::HandType PlayHand::getType()
{
	return m_type;
}

Card::CardPoint PlayHand::getPt()
{
	return m_pt;
}

int PlayHand::getExtra()
{
	return m_extra;
}

PlayHand::PlayHand()
{
}

PlayHand::PlayHand(Cards& cards)
{
	//1.对扑克牌进行分类：1张的，2张，3张，4张有多少种组合
	classify(cards);
	//2.对牌型进行分类
	judgeCardType();
}

PlayHand::PlayHand(HandType type, Card::CardPoint pt, int extra)
{
	m_type = type;
	m_pt = pt;
	m_extra = extra;
}

PlayHand::~PlayHand()
{
}
