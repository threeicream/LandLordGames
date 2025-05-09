#pragma once
#include "Player.h"
#include "PlayHand.h"

class Strategy
{
public:
	//1. 制定出牌策略
	Cards makeStrategy();
	void makeStrategyApi();
	// 2. 第一个出牌 firstPlay
	Cards firstPlay();
	// 3. 得到比指定牌型大的牌
	Cards getGreaterCards(PlayHand type);
	Cards getGreaterCards_api(PlayHand type);
	// 4. 能大过指定的牌时， 判断是出牌还是放行， 返回true->出牌， 返回false- >放行
	bool whetherToBeat(Cards& card);
	bool whetherToBeat_api(Cards& card);
	// 5.  找出指定数量（count ）的相同点数的牌（point）， 找出 count张点数为point的牌
	Cards findSamePointCards(Card::CardPoint point, int count);
	// 6. 找出所有点数数量为count的牌 ==> 得到一个多张扑克牌数组
	QVector<Cards> findCardsByCount(int count);
	// 7. 根据点数范围找牌
	Cards findRangeCards(Card::CardPoint begin, Card::CardPoint end);
	// 8. 按牌型找牌， 并且指定要找的牌是否要大过指定的牌型
	QVector<Cards> findCardType(PlayHand hand, bool beat);
	//9. 从指定的cards对象中挑选满足条件的顺子
	void pickSeqSingles(QVector< QVector<Cards>>& allSeqRecord, QVector<Cards>& seqSingle, const Cards& cards);
	//10. 最优的顺子的集合的筛选函数
	QVector<Cards> pickOptimalSeqSingle();
public:
	Strategy(Player* player, const Cards& cards);
	~Strategy();
signals:
	void cardsToPlayReady(Cards cards);
private:
	using function = Cards(Strategy::*)(Card::CardPoint point);
	struct CardInfo {
		Card::CardPoint begin;
		Card::CardPoint end;
		int extra;//顺子或连对的数量
		bool beat;
		int number;//指定点数的牌的数量1or2
		int base;//最基础的顺子或者连对的数量
		function getSeq;
	};
	QVector<Cards>getCards(Card::CardPoint point, int number);
	QVector<Cards>getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type);
	QVector<Cards>getPlane(Card::CardPoint begin);
	QVector<Cards>getPlaneTwoSingleOrTwoPair(Card::CardPoint begin, PlayHand::HandType type);
	QVector<Cards>getSeqSingleOrPair(CardInfo& info);
	Cards getBaseSeqPair(Card::CardPoint point);
	Cards getBaseSeqSingle(Card::CardPoint point);
	QVector<Cards>getBomb(Card::CardPoint begin);
	QVector<Card::CardPoint>getApiCards(Cards& cards);

	Cards convertCardsToPlay(const QVector<Card::CardPoint>& cardsToPlay);
private:
	Player* m_player;
	Cards m_cards;
	Cards ans;
	int penNum;
};
