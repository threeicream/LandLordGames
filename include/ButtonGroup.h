#pragma once

#include <QWidget>
#include "ui_ButtonGroup.h"

class ButtonGroup : public QWidget
{
	Q_OBJECT
public:
	enum Panel { START, PLAYCARD, PASSORPLAY, CALLLORD, EMPTY };
public:
	ButtonGroup(QWidget *parent = nullptr);
	~ButtonGroup();
public:
	//按钮初始化
	void initButtons();

	//处理page页的切换
	void selectPanel(Panel type ,int bet = 0);
signals:
	//开始游戏
	void startGame();
	//出牌
	void playHand();
	//不出牌
	void pass();
	//抢地主
	void betPoint(int bet);
private:
	Ui::ButtonGroup ui;
};
