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
	m_type = Hand_Unknown;
	m_pt = Card::Card_Begin;
	m_extra = 0;

	// 按照优先级顺序判断牌型
	if (isBombJokersTwoSingle()) {
		m_type = Hand_Bomb_Jokers_Two_Single;
	}
	else if (isBombJokersPair()) {
		m_type = Hand_Bomb_Jokers_Pair;
	}
	else if (isBombJokersSingle()) {
		m_type = Hand_Bomb_Jokers_Single;
	}
	else if (isBombJokers()) {
		m_type = Hand_Bomb_Jokers;
		//王炸没有点数
	}
	else if (isBombTwoSingle()) {
		m_type = Hand_Bomb_Two_Single;
		m_pt = m_fourCard.first();//炸弹大小由炸弹本身决定
	}
	else if (isBombPair()) {
		m_type = Hand_Bomb_Pair;
		m_pt = m_fourCard.first();
	}
	else if (isBombSingle()) {
		m_type = Hand_Bomb_Single;
		m_pt = m_fourCard.first();
	}
	else if (isBomb()) {
		m_type = Hand_Bomb;
		m_pt = m_fourCard.first();//记录炸弹的大小
	}
	else if (isHandSeqSingle()) {
		m_type = Hand_Hand_Seq_Single;
		m_pt = m_oneCard.first();//记录最小的点数
		m_extra = m_oneCard.size();//记录顺子的长度
	}
	else if (isHandSeqPair()) {
		m_type = Hand_Hand_Seq_Pair;
		m_pt = m_twoCard.first();//记录最小的点数
		m_extra = m_twoCard.size();//记录连对的长度
	}
	else if (isPlaneTwoPair()) {
		m_type = Hand_Plane_Two_Pair;
		m_pt = m_threeCard.first();//记录最小的点数
	}
	else if (isPlaneTwoSingle()) {
		m_type = Hand_Plane_Two_Single;
		m_pt = m_threeCard.first();
	}
	else if (isPlane()) {
		m_type = Hand_Plane;
		m_pt = m_threeCard.first(); //记录飞机最小的牌点
	}
	else if (isTriplePair()) {
		m_type = Hand_Triple_Pair;
		m_pt = m_threeCard.first();
	}
	else if (isTripleSingle()) {
		m_type = Hand_Triple_Single;
		m_pt = m_threeCard.first();
	}
	else if (isTriple()) {
		m_type = Hand_Triple;
		m_pt = m_threeCard.first(); // 记录三个的点数
	}
	else if (isPair()) {
		m_type = Hand_Pair;
		m_pt = m_twoCard.first(); // 记录对子的点数
	}
	else if (isSingle()) {
		m_type = Hand_Single;
		m_pt = m_oneCard.first(); // 记录单张的点数
	}
	else {
		m_type = Hand_Unknown; // 如果没有匹配的牌型，则保持 Hand_Unknown
	}
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

bool PlayHand::isSingle()
{
	if (m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty()) {
		return true;
	}
	return false;
}

bool PlayHand::isPair()
{
	if (m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.isEmpty()) {
		return true;
	}
	return false;
}

bool PlayHand::isTriple()
{
	if (m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.size() == 1 && m_fourCard.isEmpty()) {
		return true;
	}
	return false;
}

bool PlayHand::isTripleSingle()
{
	if (m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.size() == 1 && m_fourCard.isEmpty()) {
		return true;
	}
	return false;
}

bool PlayHand::isTriplePair()
{
	if (m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.size() == 1 && m_fourCard.isEmpty()) {
		return true;
	}
	return false;
}

bool PlayHand::isPlane()
{
	if (m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.size() == 2 && m_fourCard.isEmpty()) {
		std::sort(m_threeCard.begin(), m_threeCard.end());
		if (m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2) {
			return true;
		}
	}
	return false;
}

bool PlayHand::isPlaneTwoSingle()
{
	if (m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.size() == 2 && m_fourCard.isEmpty()) {
		std::sort(m_oneCard.begin(), m_oneCard.end());
		std::sort(m_threeCard.begin(), m_threeCard.end());
		if (m_oneCard[0] == Card::Card_SJ && m_oneCard[1] == Card::Card_BJ) {
			return false;
		}
		if (m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2) {
			return true;
		}
	}
	return false;
}

bool PlayHand::isPlaneTwoPair()
{
	if (m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.size() == 2 && m_fourCard.isEmpty()) {
		std::sort(m_threeCard.begin(), m_threeCard.end());
		if (m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2) {
			return true;
		}
	}
	return false;
}

bool PlayHand::isHandSeqPair()
{
	if (m_oneCard.isEmpty() && m_twoCard.size() >= 3 && m_threeCard.isEmpty() && m_fourCard.isEmpty()) {
		std::sort(m_twoCard.begin(), m_twoCard.end());
		if (m_twoCard.last() - m_twoCard.first() == (m_twoCard.size() - 1) 
			&& m_twoCard.first() >= Card::Card_3 && m_twoCard.last() < Card::Card_2) {
			return true;
		}
	}
	return false;
}

bool PlayHand::isHandSeqSingle()
{
	if (m_oneCard.size() >= 5 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty()) {
		std::sort(m_oneCard.begin(), m_oneCard.end());
		if (m_oneCard.last() - m_oneCard.first() == (m_oneCard.size() - 1)
			&& m_oneCard.first() >= Card::Card_3 && m_oneCard.last() < Card::Card_2) {
			return true;
		}
	}
	return false;
}

bool PlayHand::isBomb()
{
	if (m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1) {
		return true;
	}
	return false;
}

bool PlayHand::isBombSingle()
{
	if (m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1) {
		return true;
	}
	return false;
}

bool PlayHand::isBombPair()
{
	if (m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.size() == 1) {
		return true;
	}
	return false;
}

bool PlayHand::isBombTwoSingle()
{
	if (m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1) {
		std::sort(m_oneCard.begin(), m_oneCard.end());
		if (m_oneCard[0] != Card::Card_SJ && m_oneCard[1] != Card::Card_BJ)
			return true;
	}
	return false;
}

bool PlayHand::isBombJokers()
{
	if (m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1) {
		std::sort(m_oneCard.begin(), m_oneCard.end());
		if (m_oneCard[0] == Card::Card_SJ && m_oneCard[1] == Card::Card_BJ)
			return true;
	}
	return false;
}

bool PlayHand::isBombJokersSingle()
{
	if (m_oneCard.size() == 3 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1) {
		std::sort(m_oneCard.begin(), m_oneCard.end());
		if (m_oneCard[1] == Card::Card_SJ && m_oneCard[2] == Card::Card_BJ)
			return true;
	}
	return false;
}

bool PlayHand::isBombJokersPair()
{
	if (m_oneCard.size() == 2 && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.size() == 1) {
		std::sort(m_oneCard.begin(), m_oneCard.end());
		if (m_oneCard[0] == Card::Card_SJ && m_oneCard[1] == Card::Card_BJ)
			return true;
	}
	return false;
}

bool PlayHand::isBombJokersTwoSingle()
{
	if (m_oneCard.size() == 4 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1) {
		std::sort(m_oneCard.begin(), m_oneCard.end());
		if (m_oneCard[2] == Card::Card_SJ && m_oneCard[3] == Card::Card_BJ)
			return true;
	}
	return false;
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
