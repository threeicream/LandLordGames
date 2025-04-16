#include "Strategy.h"

Cards Strategy::makeStrategy()
{
	//得到出牌玩家对象以及打出的牌
	Player* penPlayer = m_player->getPendPlayer();
	Cards penCards = m_player->getPendCards();
	//判断上次出牌的玩家是不是自己
	if (penPlayer == m_player || penPlayer == nullptr) {//是，则出牌没有限制
		return firstPlay();
	}
	else {//不是，需要找比出牌玩家点数更大的牌
		PlayHand type(penCards);
		Cards beatCards = getGreaterCards(type);
		//找到点数大的牌后考虑是否出牌
		bool shouldBeat = whetherToBeat(beatCards);
		if (shouldBeat)
			return beatCards;
		else
			return Cards();
	}
	return Cards();
}

Cards Strategy::firstPlay()
{
	return Cards();
}

Cards Strategy::getGreaterCards(PlayHand type)
{
	return Cards();
}

bool Strategy::whetherToBeat(Cards& card)
{
	if (card.isEmpty())
		return false;
	//得到出牌玩家的对象
	Player* pendPlayer = m_player->getPendPlayer();
	if (m_player->getRole() == pendPlayer->getRole()) {
		//牌所剩无几且是一个完整牌型
		Cards left = m_cards;
		left.delCard(card);
		if (PlayHand(left).getType() != PlayHand::Hand_Unknown) {
			return true;
		}
		//如果cs对象中的牌的最小点数为2，大小王-->不出牌
		Card::CardPoint basePoint = PlayHand(card).getPt();
		if (basePoint >= Card::CardPoint::Card_2)
			return false;
		else{
			PlayHand myhand(card);
			//如果card为3个2带1or1对，不出牌
			if (myhand.getType() == PlayHand::Hand_Triple_Single || myhand.getType() == PlayHand::Hand_Triple_Pair 
				&& myhand.getPt() == Card::Card_2)
				return false;
			//如果card为对2，并且出牌玩家手牌>=10&&自己的牌>=5，暂时放弃出牌
			if (myhand.getType() == PlayHand::Hand_Pair && myhand.getPt() == Card::Card_2
				&& pendPlayer->getCards().cardCount() >= 10 && m_player->getCards().cardCount() >= 5)
				return false;
		}

		return true;
	}
	else {

	}
}

Cards Strategy::findSamePointCards(Card::CardPoint point, int count)
{
	if (count < 1 || count > 4)//非法
		return Cards();
	//大小王
	if (point == Card::Card_SJ || point == Card::Card_BJ) {
		if (count > 1)//非法
			return Cards();
		Card card;
		card.setPoint(point);
		card.setSuit(Card::Suit_Begin);
		if (m_cards.contains(card)) {
			Cards cards;
			cards.addCard(card);
			return cards;
		}
		else
			return Cards();
	}

	//不是大小王
	int findCount = 0;
	Cards findCards;
	for (int suit = Card::Suit_Begin + 1; suit < Card::Suit_End; ++suit) {
		Card card;
		card.setPoint(point);
		card.setSuit(static_cast<Card::CardSuit>(suit));
		if (m_cards.contains(card)) {
			++findCount;
			findCards.addCard(card);
			if (findCount == count)
				return findCards;
		}
	}
	return Cards();
}

QVector<Cards> Strategy::findCardsByCount(int count)
{
	if (count < 1 || count > 4)//非法
		return QVector<Cards>();
	QVector<Cards> cardsArray;
	for (Card::CardPoint point = Card::Card_3; point < Card::Card_End; point = static_cast<Card::CardPoint>(point + 1)) {
		if (m_cards.pointCount(point) == count) {
			Cards cs;
			cs << findSamePointCards(point, count);
			cardsArray << cs;
		}
	}
	return cardsArray;
}

QVector<Cards> Strategy::findRangeCards(Card::CardPoint begin, Card::CardPoint end)
{
	QVector<Cards> rangeCards;
	for (Card::CardPoint point = begin; point < end; point = static_cast<Card::CardPoint>(point + 1)) {
		int count = m_cards.pointCount(point);
		Cards cs = findSamePointCards(point, count);
		rangeCards << cs;
	}
	return rangeCards;
}

QVector<Cards> Strategy::findCardType(PlayHand hand, bool beat)
{
	PlayHand::HandType type = hand.getType();
	Card::CardPoint pt = hand.getPt();
	int extra = hand.getExtra();
	
	//确认起始点数
	Card::CardPoint beginPoint = beat ? Card::CardPoint(pt + 1) : Card::Card_3;

	switch (type)
	{
	case PlayHand::Hand_Single: 
		return getCards(beginPoint, 1);
	case PlayHand::Hand_Pair:
		return getCards(beginPoint, 2);
	case PlayHand::Hand_Triple:
		return getCards(beginPoint, 3);
	case PlayHand::Hand_Triple_Single:
		return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Single);
	case PlayHand::Hand_Triple_Pair:
		return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Pair);
	case PlayHand::Hand_Plane:
		return getPlane(beginPoint);
	case PlayHand::Hand_Plane_Two_Single:
		return getPlaneTwoSingleOrTwoPair(beginPoint, PlayHand::Hand_Single);
	case PlayHand::Hand_Plane_Two_Pair:
		return getPlaneTwoSingleOrTwoPair(beginPoint, PlayHand::Hand_Pair);
	case PlayHand::Hand_Seq_Pair:
	{
		CardInfo info;
		info.begin = beginPoint;
		info.end = Card::CardPoint::Card_Q;
		info.number = 2;
		info.base = 3;
		info.extra = extra;
		info.beat = beat;
		info.getSeq = &Strategy::getBaseSeqPair;
		return getSeqSingleOrPair(info);
	}
	case PlayHand::Hand_Seq_Single:
	{
		CardInfo info;
		info.begin = beginPoint;
		info.end = Card::CardPoint::Card_10;
		info.number = 1;
		info.base = 5;
		info.extra = extra;
		info.beat = beat;
		info.getSeq = &Strategy::getBaseSeqSingle;
		return getSeqSingleOrPair(info);
	}
	case PlayHand::Hand_Bomb:
		return getBomb(beginPoint);
	default:
		return QVector<Cards>();
	}
}

Strategy::Strategy(Player* player, const Cards& cards)
{
	m_player = player;
	m_cards = cards;
}

Strategy::~Strategy()
{
}

QVector<Cards> Strategy::getCards(Card::CardPoint point, int number)
{
	QVector<Cards>findCardsArray;
	for (Card::CardPoint pt = point; pt < Card::Card_End; pt = static_cast<Card::CardPoint>(pt + 1)) {
		Cards cs = findSamePointCards(pt, number);
		if (!cs.isEmpty())
			findCardsArray << cs;
	}
	return findCardsArray;
}

QVector<Cards> Strategy::getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type)
{
	//找到点数相同的三张牌
	QVector<Cards>findCardsArray = getCards(begin, 3);
	if(findCardsArray.isEmpty())
		return QVector<Cards>();
	//将找到的牌从用户手中删除
	Cards remainCards = m_cards;
	remainCards.delCard(findCardsArray);
	//搜索单牌或者成对的牌
	Strategy st(m_player, remainCards);
	QVector<Cards> cardsArray = st.findCardType(PlayHand(type, Card::CardPoint::Card_Begin, 0), false);
	if (!cardsArray.isEmpty()) {
		//将找到的牌与三张点数相同的牌组合
		for (int i = 0; i < findCardsArray.size(); ++i) {
			findCardsArray[i].addCard(cardsArray[0]);
		}
	}
	else {
		findCardsArray.clear();
	}
	//将最终结果返回给函数调用者
	return findCardsArray;
}

QVector<Cards> Strategy::getPlane(Card::CardPoint begin)
{
	QVector<Cards>findCardArray;
	for (Card::CardPoint point = begin; point < Card::Card_A; point = static_cast<Card::CardPoint>(point + 1)) {
		//根据点数和数量进行搜索
		Cards prevCards = findSamePointCards(point, 3);
		Cards nextCards = findSamePointCards(static_cast<Card::CardPoint>(point + 1), 3);
		if (!prevCards.isEmpty() && !nextCards.isEmpty()) {
			Cards cards;
			cards << prevCards << nextCards;
			findCardArray << cards;
		}
	}
	return findCardArray;
}

QVector<Cards> Strategy::getPlaneTwoSingleOrTwoPair(Card::CardPoint begin, PlayHand::HandType type)
{
	//找到飞机
	QVector<Cards>findCardsArray = getPlane(begin);
	if (findCardsArray.isEmpty())
		return QVector<Cards>();
	//将找到的牌从用户手中删除
	Cards remainCards = m_cards;
	remainCards.delCard(findCardsArray);
	//搜索单牌或者成对的牌
	Strategy st(m_player, remainCards);
	QVector<Cards> cardsArray = st.findCardType(PlayHand(type, Card::CardPoint::Card_Begin, 0), false);
	if (cardsArray.size() >= 2) {
		//将找到的牌与三张点数相同的牌组合
		for (int i = 0; i < findCardsArray.size(); ++i) {
			Cards cards;
			cards << cardsArray[0] << cardsArray[1];
			findCardsArray[i].addCard(cards);
		}
	}
	else {
		findCardsArray.clear();
	}
	//将最终结果返回给函数调用者
	return findCardsArray;
}

QVector<Cards> Strategy::getSeqSingleOrPair(CardInfo& info)
{
	QVector<Cards>findCardsArray;
	if (info.beat) {
		//连对最少三个，最大A
		for (Card::CardPoint point = info.begin; point <= info.end; point = static_cast<Card::CardPoint>(point + 1)) {
			bool found = true;
			Cards seqCards;
			for (int i = 0; i < info.extra; ++i) {
				//基于点数和数量进行牌的搜索
				Cards cards = findSamePointCards(static_cast<Card::CardPoint>(point + i), info.number);
				if (cards.isEmpty() || (point + info.extra >= Card::CardPoint::Card_2)) {
					found = false;
					seqCards.clear();
					break;
				}
				else {
					seqCards << cards;
				}
			}
			if (found) {
				findCardsArray << seqCards;
				return findCardsArray;
			}
		}
	}
	else {
		for (Card::CardPoint point = info.begin; point <= info.end; point = static_cast<Card::CardPoint>(point + 1)) {
			//将找到的基础连对存储
			Cards baseSeq = (this->*info.getSeq)(point);
			if (baseSeq.isEmpty())continue;
			//将基础连对存储到容器
			findCardsArray << baseSeq;
			 
			//继续搜索
			int followed = info.base;
			Cards alreadyFollowedCards;//存储后序找到的满足条件的连对
			while (true) {
				//新的起始点数
				Card::CardPoint followedPoint = static_cast<Card::CardPoint>(point + followed);
				//判断点数是否合法
				if (followedPoint >= Card::Card_2)
					break;
				Cards followedCards = findSamePointCards(followedPoint, info.number);
				if (followedCards.isEmpty())
					break;
				alreadyFollowedCards << followedCards;
				Cards newSeq = baseSeq;
				newSeq << alreadyFollowedCards;
				findCardsArray << newSeq;
				++followed;
			}
		}
	}
	return findCardsArray;
}

Cards Strategy::getBaseSeqPair(Card::CardPoint point)
{
	Cards card0 = findSamePointCards(point, 2);
	Cards card1 = findSamePointCards(static_cast<Card::CardPoint>(point + 1), 2);
	Cards card2 = findSamePointCards(static_cast<Card::CardPoint>(point + 2), 2);
	Cards baseSeq;
	if (!card0.isEmpty() || !card1.isEmpty() || !card2.isEmpty())
		baseSeq << card0 << card1 << card2;
	return baseSeq;
}

Cards Strategy::getBaseSeqSingle(Card::CardPoint point)
{
	Cards card0 = findSamePointCards(point, 1);
	Cards card1 = findSamePointCards(static_cast<Card::CardPoint>(point + 1), 1);
	Cards card2 = findSamePointCards(static_cast<Card::CardPoint>(point + 2), 1);
	Cards card3 = findSamePointCards(static_cast<Card::CardPoint>(point + 3), 1);
	Cards card4 = findSamePointCards(static_cast<Card::CardPoint>(point + 4), 1);
	Cards baseSeq;
	if (!card0.isEmpty() || !card1.isEmpty() || !card2.isEmpty() || !card3.isEmpty() || !card4.isEmpty())
		baseSeq << card0 << card1 << card2 << card3 << card4;
	return baseSeq;
}

QVector<Cards> Strategy::getBomb(Card::CardPoint begin)
{
	QVector<Cards>findcardsArray;
	for (Card::CardPoint point = Card::Card_3; point < Card::Card_End; point = static_cast<Card::CardPoint>(point + 1)) {
		Cards cs = findSamePointCards(point, 4);
		if (!cs.isEmpty()) {
			findcardsArray << cs;
		}
	}
	return findcardsArray;
}
