#pragma once

#include <QWidget>
#include "ui_gamePanel.h"
#include <QPixmap>
#include <QVector>
#include <QSize>
#include "Card.h"
class QPaintEvent;
class GameControl;
class Player;
class CardPanel;
#include <QMouseEvent>

class gamePanel : public QWidget
{
	Q_OBJECT

public:
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
protected:
	virtual void paintEvent(QPaintEvent* event)override;

	void mousePressEvent(QMouseEvent* event) override {
		QPoint pos = event->pos(); // 获取相对于窗口客户区域的坐标
		qDebug() << "Mouse pressed at:" << pos;
		// 如果你想获取相对于屏幕的坐标，可以使用 event->globalPos()
		QPoint globalPos = event->globalPos();
		qDebug() << "Mouse pressed at (global):" << globalPos;
	}
private:
	Ui::gamePanel ui;
private:
	QPixmap m_backGroundPixmap;
	GameControl* m_gameCtl;
	QVector<Player*>m_playerList;
	QMap<Card, CardPanel*>m_cardMap;
	QSize m_cardSize;
	QPixmap m_cardBackImg;
};
