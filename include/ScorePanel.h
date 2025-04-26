#pragma once

#include <QWidget>
#include "ui_ScorePanel.h"

class ScorePanel : public QWidget
{
	Q_OBJECT

public:
	ScorePanel(QWidget* parent = nullptr);
	~ScorePanel();

	enum FontColor { Black, White, Red, Blue, Green };
	//修改得分
	void setScore(int PlayerNum,int LeftNum, int RightNum);
	//设置字体的颜色
	void setMyFontColor(ScorePanel::FontColor color);
	//设置字体的大小
	void setMyFontSize(int point);
private:
	Ui::ScorePanel ui;

	QVector<QLabel*>m_list;
};
