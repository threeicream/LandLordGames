#pragma once

#include <QObject>
#include "Robot.h"
#include "UserPlayer.h"
#include "Cards.h"

struct BetRecord
{
	BetRecord() {
		reset();
	}

	void reset() {//初始化
		player = nullptr;
		bet = 0;
		times = 0;
	}

	Player* player;
	int bet;
	int times;//第几次叫地主
};

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
	void becomeLord(Player* player, int bet);
	//清空得分
	void clearPlayerScore();
	//得到玩家下注的最高分数
	int getPlayerMaxBet();

public slots:
	//处理叫地主
	void onGrabBet(Player* player, int bet);
	//处理出牌
	void onPlayHand(Player* player, Cards& cards);
signals:
	void playerStatusChanged(Player* player, PlayerStatus status);
	//通知玩家抢地主
	void notifyGrabLordBet(Player* player, int bet, bool flag);
	//游戏状态变化
	void gameStatusChanged(GameStatus status);
	//通知玩家出牌
	void notifyPlayHand(Player* player, Cards& cards);
	//给玩家传递出牌数据
	void pendingInfo(Player* player, Cards& cards);
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
	BetRecord m_betRecord;
	int m_curBet;
};
