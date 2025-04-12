#pragma once
#include "Cards.h"

class PlayHand
{
public:
	//出牌组合或方式
	enum HandType {
		Hand_Unknown,//未知
		Hand_Pass,//过

		Hand_Single,//单
		Hand_Pair,//对

		Hand_Triple,//三个
		Hand_Triple_Single,//三带一
		Hand_Triple_Pair,//三带二

		Hand_Plane,//飞机 555_666
		Hand_Plane_Two_Single,//飞机带单 555_666_3_4
		Hand_Plane_Two_Pair,//飞机带双 555_666_33_44

		Hand_Hand_Seq_Pair,//连对 33_44_55
		Hand_Hand_Seq_Single,//顺子 34567

		Hand_Bomb,//炸弹
		Hand_Bomb_Single,//炸弹带一个
		Hand_Bomb_Pair,//炸弹带一对
		Hand_Bomb_Two_Single,//炸弹带两单

		Hand_Bomb_Jokers,//王炸
		Hand_Bomb_Jokers_Single,//王炸带一个
		Hand_Bomb_Jokers_Pair,//王炸带一对
		Hand_Bomb_Jokers_Two_Single//王炸带两单
	};
public:
	//1.对扑克牌进行分类：1张的，2张，3张，4张有多少种组合
	void classify(Cards& cards);

	//2.对牌型进行分类
	void judgeCardType();

	//得到牌型的属性信息
	PlayHand::HandType getType();
	Card::CardPoint getPt();
	int getExtra();
public:
	PlayHand();
	//传递给类一组牌，通过类分析出牌型，点数，以及相关的附属信息（比如顺子：记录牌的数量）
	PlayHand(Cards& cards);
	PlayHand(HandType type, Card::CardPoint pt, int extra);
	~PlayHand();
private:
	HandType m_type;
	Card::CardPoint m_pt;
	int m_extra;
};
