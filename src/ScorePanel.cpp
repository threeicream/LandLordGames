#include "ScorePanel.h"

ScorePanel::ScorePanel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

}

ScorePanel::~ScorePanel()
{
}

void ScorePanel::setScore(int PlayerNum, int LeftNum, int RightNum)
{
	ui.PlayerScore->setText(QString::number(PlayerNum));
	ui.leftScore->setText(QString::number(LeftNum));
	ui.rightScore->setText(QString::number(RightNum));
}