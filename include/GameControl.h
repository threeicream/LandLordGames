#pragma once

#include <QObject>
#include "Robot.h"
#include "UserPlayer.h"
#include "Cards.h"

class GameControl : public QObject
{
	Q_OBJECT
public:
	//游戏状态
	enum GameStatus {
		DISPATCHCARD,//发牌
		CALLINGLORD,//叫地主
		PLAYINGHAND//出牌
	};
	//玩家状态
	enum PlayerStatus {
		THINKCALLLORD,//叫地主
		THINKPLAYHAND,//出牌
		WINNING//胜利
	};
public:
	//初始化玩家
	void playerInit();
	//得到玩家的实例对象
	Robot* getLeftRobot()const;
	Robot* getRightRobot()const;
	UserPlayer* getUserPlayer()const;

	void setCurrentPlayer(Player* player);
	Player* getCurrentPlayer()const;
	//得到出牌玩家和打出的牌
	Player* getPendPlayer()const;
	Cards getPendCards()const;
	
	//初始化扑克牌
	void allCardsInit();
	//每次发一张牌
	Card takeOneCard();
	//得到最后三张底牌
	Cards getSurplusCards()const;
	//重置卡牌数据
	void resetCardData();

	//准备叫地主
	void startLordCard();
	//成为地主
	void becomeLord(Player* player);
	//清空得分
	void clearPlayerScore();

	//处理叫地主

	//处理出牌
	
public:
	GameControl(QObject *parent);
	~GameControl();
private:
	Robot* m_robotLeft;
	Robot* m_robotRight;
	UserPlayer* m_user;
	Player* m_currPlayer;
	Player* m_pendPlayer;
	Cards m_pendCards;
	Cards m_allCards;
};
