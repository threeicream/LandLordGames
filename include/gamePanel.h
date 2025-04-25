#pragma once

#include "Card.h"
#include "ui_gamePanel.h"
#include <QPixmap>
#include <QSize>
#include <QVector>
#include <QWidget>
#include <QMouseEvent>
#include "Cards.h"
#include "GameControl.h"
#include <QSet>
class QPaintEvent;
class Player;
class CardPanel;
class QLabel;
class QTimer;
class AnimationWindow;

class gamePanel : public QWidget
{
	Q_OBJECT

public:
	enum CardAlign { HOR, VER };
	enum AnimationType { SHUNZI, LIANDUI, PLANE, JOKERBOMB, BOMB, BET };
	gamePanel(QWidget* parent = nullptr);
	~gamePanel();

	//游戏控制类初始化
	void gameControlInit();
	//更新面板分数
	void UpdateScorePanel();
	//切割并存储图片
	void CardMapInit();
	//裁剪图片
	void cropImg(QPixmap& pix, int x, int y, Card& card);
	//按钮组初始化
	void buttonGroupInit();
	//玩家在窗口中的上下文环境初始化
	void PlayerContextInit();
	//初始化游戏场景
	void GameSceneInit();
	//处理游戏的状态
	void gameStatusProcess(GameControl::GameStatus status);
	//发牌
	void startDispatchCard();
	//定时器的处理动作
	void onDispatchCard();
	//移动扑克牌
	void cardMoveStep(Player* player, int curPos);
	void zoomY(int y);
	//处理分发得到的牌
	void disposCard(Player* player,Cards& cards);
	//更新扑克牌在窗口的显示
	void updatePlayerCards(Player* player);

	//处理玩家的出牌
	void onDisposePlayHand(Player* player, Cards& cards);

	//显示特效动画
	void showAnimation(AnimationType type, int bet = 0);
	//隐藏玩家打出的牌
	void hidePlayerDropCards(Player* player);
	//加载玩家头像
	QPixmap loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role);
	
public slots:
	//处理玩家状态的变化
	void onPlayerStatusChanged(Player* player, GameControl::PlayerStatus status);
	//处理抢地主点数
	void onGrabLordBet(Player* player, int bet, bool flag);
	//处理玩家选牌
	void onCardSelected(Qt::MouseButton button);
	//处理玩家出牌
	void onUserPlayHand();
	//处理玩家跳过
	void onUserPass();
protected:
	virtual void paintEvent(QPaintEvent* event)override;

	virtual void mouseMoveEvent(QMouseEvent* event)override;

	//virtual void mousePressEvent(QMouseEvent* event) override {
	//	QPoint pos = event->pos(); // 获取相对于窗口客户区域的坐标
	//	qDebug() << "Mouse pressed at:" << pos;
	//	// 如果你想获取相对于屏幕的坐标，可以使用 event->globalPos()
	//	QPoint globalPos = event->globalPos();
	//	qDebug() << "Mouse pressed at (global):" << globalPos;
	//}
private:
	Ui::gamePanel ui;
private:
	struct PlayerContext {
		//1.玩家扑克牌显示区域
		QRect cardRect;
		//2.出牌的区域
		QRect PlayHandRect;
		//3.扑克牌的对齐方式（水平/垂直）
		CardAlign align;
		//4.扑克牌显示正面还是背面
		bool isFrontSide;
		//5.游戏过程中的提示信息：如不出
		QLabel* info;
		//6.玩家的头像
		QLabel* roleImg;
		//7.玩家刚打出的牌
		Cards lastCards;
	};
	QPixmap m_backGroundPixmap;
	GameControl* m_gameCtl = nullptr;
	QVector<Player*>m_playerList;
	QMap<Card, CardPanel*>m_cardMap;
	QSize m_cardSize;
	QPixmap m_cardBackImg;
	QMap<Player*, PlayerContext>m_contextMap;
	CardPanel* m_baseCard;
	CardPanel* m_moveCard;
	QVector<CardPanel*>m_last3Card;
	QPoint m_baseCardPos;
	GameControl::GameStatus m_gameStatus;
	QTimer* m_timer = nullptr;
	AnimationWindow* m_animation = nullptr;
	CardPanel* m_curSelCard = nullptr;
	QSet<CardPanel*>m_selectCards;//选择的牌
	QRect m_cardsRect;//出牌区域
	QHash<CardPanel*, QRect>m_userCards;
};
