#pragma once

#include <QWidget>
#include "ui_gamePanel.h"
#include <QPixmap>
#include <QVector>
class QPaintEvent;
class GameControl;
class Player;

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
protected:
	virtual void paintEvent(QPaintEvent* event)override;
private:
	Ui::gamePanel ui;
private:
	QPixmap m_backGroundPixmap;
	GameControl* m_gameCtl;
	QVector<Player*>m_playerList;
};
