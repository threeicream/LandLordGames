#include "Strategy.h"
#include <QMap>

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
	//判断顽疾手中是否只剩单一的牌型
	PlayHand hand(m_cards);
	if (hand.getType() != PlayHand::Hand_Unknown) {
		return m_cards;
	}
	else {//不是单一牌型
		//判断玩家手中是否有顺子
		QVector<Cards>optimalSeq = pickOptimalSeqSingle();
		if (!optimalSeq.isEmpty()) {
			//得到单牌的数量
			int baseNum = findCardsByCount(1).size();
			//删除得到的顺子
			Cards temp = m_cards;
			temp.delCard(optimalSeq);
			int lastNum = Strategy(m_player, temp).findCardsByCount(1).size();
			if (baseNum > lastNum) {
				return optimalSeq[0];
			}
		}
	}

	bool hasPlane, hasTriple, hasPair;
	hasPlane = hasTriple = hasPair = false;
	Cards backup = m_cards;
	
	//有没有炸弹
	QVector<Cards>seqBombArray = findCardType(PlayHand(PlayHand::Hand_Bomb, Card::Card_Begin, 0), false);
	backup.delCard(seqBombArray);
	//有没有飞机
	QVector<Cards>seqPlaneArray = Strategy(m_player,backup).findCardType(PlayHand(PlayHand::Hand_Plane, Card::Card_Begin, 0), false);
	if (!seqPlaneArray.isEmpty()) {
		hasPlane = true;
		backup.delCard(seqPlaneArray);
	}
	//有没有三带
	QVector<Cards>seqTripleArray = Strategy(m_player, backup).findCardType(PlayHand(PlayHand::Hand_Triple, Card::Card_Begin, 0), false);
	if (!seqTripleArray.isEmpty()) {
		hasTriple = true;
		backup.delCard(seqTripleArray);
	}
	//有没有连对
	QVector<Cards>seqPairArray = Strategy(m_player, backup).findCardType(PlayHand(PlayHand::Hand_Seq_Pair, Card::Card_Begin, 0), false);
	if (!seqPairArray.isEmpty()) {
		hasPair = true;
		backup.delCard(seqPairArray);
	}

	int maxNum = INT_MIN;
	int seq = 0;
	if (hasPair) {
		for (int i = 0; i < seqPairArray.size(); ++i) {
			if (seqPairArray[i].cardCount() > maxNum) {
				maxNum = seqPairArray[i].cardCount();
				seq = i;
			}
		}
		return seqPairArray[seq];
	}

	if (hasPlane) {
		//1.飞机带两个对
		bool twoPairFond = false;
		QVector<Cards> pairArray;
		for (Card::CardPoint point = Card::Card_3; point <= Card::Card_10; point = Card::CardPoint(point + 1)) {
			Cards pair = Strategy(m_player, backup).findSamePointCards(point,2);
			if (!pair.isEmpty()) {
				pairArray.push_back(pair);
			}
			if (pairArray.size() == 2) {
				twoPairFond = true;
				break;
			}
		}
		if (twoPairFond) {
			Cards tmp = seqPlaneArray[0];
			tmp.addCard(pairArray);
			return tmp;
		}
		else {//2.飞机带两个单
			bool twoSingleFond = false;
			QVector<Cards> singleArray;
			for (Card::CardPoint point = Card::Card_3; point <= Card::Card_10; point = Card::CardPoint(point + 1)) {
				if (backup.pointCount(point) == 1) {//确认这个点数的牌是否为单牌
					Cards single = Strategy(m_player, backup).findSamePointCards(point, 1);
					if (!single.isEmpty()) {
						singleArray.push_back(single);
					}
					if (singleArray.size() == 2) {
						twoSingleFond = true;
						break;
					}
				}
			}
			if (twoSingleFond) {
				Cards tmp = seqPlaneArray[0];
				tmp.addCard(singleArray);
				return tmp;
			}
			else {//3.飞机
				return pairArray[0];
			}
		}
	}

	if (hasTriple) {
		if (PlayHand(seqTripleArray[0]).getPt() < Card::Card_J) {//点数过大就不出
			for (Card::CardPoint point = Card::Card_3; point <= Card::Card_A; point = Card::CardPoint(point + 1)) {
				int pointCount = backup.pointCount(point);
				if (pointCount == 1) {
					Cards single = Strategy(m_player, backup).findSamePointCards(point, 1);
					Cards tmp = seqTripleArray[0];
					tmp.addCard(single);
					return tmp;
				}
				else if (pointCount == 2) {
					Cards pair = Strategy(m_player, backup).findSamePointCards(point, 2);
					Cards tmp = seqTripleArray[0];
					tmp.addCard(pair);
					return tmp;
				}
			}
		}
		//不带副牌
		return seqTripleArray[0];
	}

	//单牌或者对牌（不需要判断下家还有几张牌，因为是第一次出牌）
	Player* nextPlayer = m_player->getNextPlayer();
	if (m_player->getRole() != nextPlayer->getRole()) {
		for (Card::CardPoint point = Card::Card_BJ; point >= Card::Card_3; point = Card::CardPoint(point - 1)) {
			int pointCount = backup.pointCount(point);
			if (pointCount == 1) {
				Cards single = Strategy(m_player, backup).findSamePointCards(point, 1);
				return single;
			}
			else if (pointCount == 2) {
				Cards pair = Strategy(m_player, backup).findSamePointCards(point, 2);
				return pair;
			}
		}
	}
	else {
		for (Card::CardPoint point = Card::Card_3; point <= Card::Card_BJ; point = Card::CardPoint(point + 1)) {
			int pointCount = backup.pointCount(point);
			if (pointCount == 1) {
				Cards single = Strategy(m_player, backup).findSamePointCards(point, 1);
				return single;
			}
			else if (pointCount == 2) {
				Cards pair = Strategy(m_player, backup).findSamePointCards(point, 2);
				return pair;
			}
		}
	}
	return Cards();//这个是不可能的
}

Cards Strategy::getGreaterCards(PlayHand type)
{
	//1.出牌玩家和当前玩家阵营不一致
	Player* penPlayer = m_player->getPendPlayer();
	if (penPlayer->getRole() != m_player->getRole() && penPlayer->getCards().cardCount() <= 3) {
		QVector<Cards>bombs = findCardsByCount(4);
		for (int i = 0; i < bombs.size(); ++i) {
			PlayHand hand(bombs[i]);
			if (hand.canBeat(type)) {
				return bombs[i];
			}
		}
	}
	//2.下一个玩家和当前玩家阵营不一致
	Player* nextPlayer = m_player->getNextPlayer();
	//将玩家手中的顺子剔除出去
	Cards remain = m_cards;
	remain.delCard(Strategy(m_player, remain).pickOptimalSeqSingle());//移除顺子

	QVector<Cards>beatCardsArray = Strategy(m_player, remain).findCardType(type, true);
	if (!beatCardsArray.isEmpty()) {
		if (nextPlayer->getRole() != m_player->getRole() && nextPlayer->getCards().cardCount() <= 2) {
			return beatCardsArray.back();
		}
		else {
			return beatCardsArray.front();
		}
	}
	else {
		QVector<Cards>beatCardsArray1 = Strategy(m_player, m_cards).findCardType(type, true);
		if (!beatCardsArray1.isEmpty()) {
			if (nextPlayer->getRole() != m_player->getRole() && nextPlayer->getCards().cardCount() <= 2) {
				return beatCardsArray1.back();
			}
			else {
				return beatCardsArray1.front();
			}
		}
	}
	return Cards();
}

Cards Strategy::getGreaterCards_api(PlayHand type)
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
	}
	return true;
}

bool Strategy::whetherToBeat_api(Cards& card)
{
	return false;
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

Cards Strategy::findRangeCards(Card::CardPoint begin, Card::CardPoint end)
{
	Cards rangeCards;
	for (Card::CardPoint point = begin; point <= end; point = static_cast<Card::CardPoint>(point + 1)) {
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

void Strategy::pickSeqSingles(QVector< QVector<Cards>>& allSeqRecord , QVector<Cards>& seqSingle, const Cards& cards)
{
	//1.得到所有的顺子组合
	QVector<Cards> allSeq = Strategy(m_player, cards).findCardType(PlayHand(PlayHand::Hand_Seq_Single, Card::Card_Begin, 0), false);
	//for (auto& it : allSeq) {//测试用
	//	it.getAllCardsPoint();
	//}
	if (allSeq.isEmpty()) {
		//递归结束条件，将满足条件的顺子传递给调用者
		allSeqRecord << seqSingle;
	}
	else {//2.对顺子进行筛选
		//遍历得到的所有顺子
		Cards saveCards = cards;
		for (int i = 0; i < allSeq.size(); ++i) {
			//将顺子取出
			Cards aScheme = allSeq[i];
			//将顺子从用户手中删除
			Cards temp = saveCards;
			temp.delCard(aScheme);

			QVector<Cards> seqArray = seqSingle;
			seqArray << aScheme;
			//检测还有没有其他顺子
			//seqArray储存一轮for循环中多轮递归得到的可用的顺子
			//allSeqRecord储存多轮for循环中多轮递归得到的可用的顺子
			pickSeqSingles(allSeqRecord, seqArray, temp);//递归
		}
	}
	

}

QVector<Cards> Strategy::pickOptimalSeqSingle()
{
	QVector<QVector<Cards>>seqRecord;
	QVector<Cards>seqSingles;
	Cards save = m_cards;
	//避免删除炸弹和三带
	save.delCard(findCardsByCount(4));
	save.delCard(findCardsByCount(3));
	pickSeqSingles(seqRecord, seqSingles, save);

	if (seqRecord.isEmpty())
		return QVector<Cards>();

	
	QMap<int, int>seqMarks;
	int minSeq = 0;
	int minCount = INT_MAX;
	for (int i = 0; i < seqRecord.size(); ++i) {
		Cards temp = m_cards;
		temp.delCard(seqRecord[i]);

		//判断剩下的单牌数量
		QVector<Cards> singleArray = Strategy(m_player, temp).findCardsByCount(1);

		CardLsit list;//单牌容器
		for (int j = 0; j < singleArray.size(); ++j) {
			list << singleArray[j].toCardList();
		}
		//找点数相对较大的顺子
		int mark = 0;
		for (int k = 0; k < list.size(); ++k) {
			mark += list[k].getPoint() + 15;//15是基准数，为了避免特殊情况 3，7||k
		}
		//seqMarks[i] = mark;
		if (minCount > mark) {
			minCount = mark;
			minSeq = i;
		}
	}
	
	/*//遍历map？真的需要用map吗？
	int comMark = INT_MAX;
	int value = 0;
	auto it = seqMarks.constBegin();
	for (; it != seqMarks.constEnd(); ++it) {
		if (it.value() < comMark) {
			comMark = it.value();
			value = it.key();
		}
	}*/

	return seqRecord[minSeq];
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
		//目的是尽量不拆分其他的牌型
		if (m_cards.pointCount(pt) == number) {
			Cards cs = findSamePointCards(pt, number);
			findCardsArray << cs;
		}
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
	if (!card0.isEmpty() && !card1.isEmpty() && !card2.isEmpty() && !card3.isEmpty() && !card4.isEmpty())
		baseSeq << card0 << card1 << card2 << card3 << card4;
	return baseSeq;
}

QVector<Cards> Strategy::getBomb(Card::CardPoint begin)
{
	QVector<Cards>findcardsArray;
	for (Card::CardPoint point = begin; point < Card::Card_End; point = static_cast<Card::CardPoint>(point + 1)) {
		Cards cs = findSamePointCards(point, 4);
		if (!cs.isEmpty()) {
			findcardsArray << cs;
		}
	}
	return findcardsArray;
}
