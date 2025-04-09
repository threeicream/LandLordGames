#pragma once

#include <QObject>
#include <QString>
#include "Cards.h"
#include <QScopedPointer>

class Player : public QObject
{
	Q_OBJECT
public:
	enum Role { FARMER, LANDLORD };
	enum Sex{MAN,WOMAN};
	enum Direction { LEFT, RIGHT };
	enum Type { ROBOT, USER, UNKNOW };

	//名字
	void setName(QString name);
	QString getName()const;
	//角色
	void setRole(Player::Role role);
	Player::Role getRole()const;
	//性别
	void setSex(Player::Sex sex);
	Player::Sex getSex()const;
	//方位
	void setDirection(Player::Direction direction);
	Player::Direction getDirection()const;
	//类别
	void setType(Player::Type type);
	Player::Type getType()const;
	//分数
	void setScore(int score);
	int getScore()const;
	//游戏结果
	void setIsWin(bool isWin);
	bool getIsWin()const;

	//提供当前对象的上/下家对象
	void setPrevPlayer(Player* player);
	void setNextPlayer(Player* player);
	Player* getPrevPlayer() const;
	Player* getNextPlayer() const;

	//叫地主/抢地主
	void grabLordBet(int point);

	//存储牌（发牌时获得）
	void storeDispatchCard(Card& card);
	void storeDispatchCards(Cards& cards);

	//得到所有的牌
	Cards getCards()const;
	//清空手牌
	void clearCards();
	//出牌
	void playHand(Cards& cards);

	//设置出牌的玩家已经待处理的牌
	void setPendingInfo(Player* player, const Cards& cards);
	Player* getPendPlayer()const;
	Cards getPendCards()const;

	//虚函数
	virtual void prepareCallLord();
	virtual void preparePlayHand();
public:
	Player(QObject* parent = nullptr);
	Player(QString name,QObject* parent = nullptr);
	~Player();
protected:
	QString m_name;
	Role m_role;
	Sex m_sex;
	Direction m_direction;
	Type m_tyep;
	int m_score = 0;
	bool m_isWin;
	Player* m_PrevPlayer;
	Player* m_NextPlayer;
	Cards m_cards;//存储玩家牌
	Cards m_pendCards;
	Player* pendPlayer;
};
