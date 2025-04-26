#include "ScorePanel.h"

const QString MYCOLOR[] = { "black","white","red","blue","green" };

ScorePanel::ScorePanel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_list << ui.label << ui.label_2 << ui.label_3 << ui.label_4 << ui.label_5 << ui.label_6 << ui.leftScore << ui.rightScore << ui.PlayerScore;
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

void ScorePanel::setMyFontColor(ScorePanel::FontColor color)
{
	QString styleSheet = QString("QLabel{color:%1}").arg(MYCOLOR[color]);
	for (auto& it : m_list) {
		it->setStyleSheet(styleSheet);
	}
}

void ScorePanel::setMyFontSize(int point)
{
	QFont font("微软雅黑", point, QFont::Bold);
	for (auto& it : m_list) {
		it->setFont(font);
	}
	
}
