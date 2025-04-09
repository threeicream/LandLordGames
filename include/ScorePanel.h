#pragma once

#include <QWidget>
#include "ui_ScorePanel.h"

class ScorePanel : public QWidget
{
	Q_OBJECT

public:
	ScorePanel(QWidget* parent = nullptr);
	~ScorePanel();

	//修改得分
	void setScore(int PlayerNum,int LeftNum, int RightNum);
private:
	Ui::ScorePanel ui;
};
