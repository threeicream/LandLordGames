#include "EndingPanel.h"
#include <QLabel>
#include "ScorePanel.h"
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QPropertyAnimation>

EndingPanel::EndingPanel(bool isLord, bool isWin, QWidget *parent)
	: QWidget(parent)
{
	// 创建样式表字符串
	//QString styleSheet = QString("QWidget {border-image: url(:/LordGame/image/gameover.png);}");
	//setStyleSheet(styleSheet);
	m_picture.load(":/LordGame/image/gameover.png");
	setFixedSize(400, 500);

	QString tileSheet;
	//显示用户玩家的角色以及游戏状态
	m_title = new QLabel(this);
	if (isLord && isWin) {
		tileSheet = QString("border-image: url(:/LordGame/image/lord_win.png);");
	}
	else if (isLord && !isWin) {
		tileSheet = QString("border-image: url(:/LordGame/image/lord_fail.png);");
	}
	else if (!isLord && isWin) {
		tileSheet = QString("border-image: url(:/LordGame/image/farmer_win.png);");
	}
	else if (!isLord && !isWin) {
		tileSheet = QString("border-image: url(:/LordGame/image/farmer_fail.png);");
	}
	m_title->setStyleSheet(tileSheet);
	m_title->setFixedSize(145, 50);
	m_title->move(125, 125);

	//分数面板
	m_score = new ScorePanel(this);
	m_score->move(75, 230);
	m_score->setFixedSize(260, 160);
	m_score->setMyFontColor(ScorePanel::Red);
	m_score->setMyFontSize(18);

	//继续游戏
	m_continue = new QPushButton(this);
	m_continue->move(84, 429);
	QString continueSytle = QString("QPushButton {"
		"    border-image: url(:/LordGame/image/button_normal.png);"
		"    border: none;" // 移除边框，非常重要！
		"}"
		"QPushButton:hover {"
		"    border-image: url(:/LordGame/image/button_hover.png);"
		"}"
		"QPushButton:pressed {"
		"    border-image: url(:/LordGame/image/button_pressed.png);"
		"}");
	m_continue->setStyleSheet(continueSytle);
	m_continue->setFixedSize(231, 48);

	connect(m_continue, &QPushButton::clicked, this, &EndingPanel::continueGame);
}

EndingPanel::~EndingPanel()
{
}

void EndingPanel::setMyScore(int left, int right, int user)
{
	m_score->setScore(user, left, right);
}

void EndingPanel::zoomY(int y)
{
	//创建动画对象
	QPropertyAnimation* animation1 = new QPropertyAnimation(this, "pos",this);//geometry
	//设置时间间隔，单位毫秒
	animation1->setDuration(1500);
	//创建起始位置
	//animation1->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
	animation1->setStartValue(this->pos());
	//创建结束位置
	//animation1->setEndValue(QRect(this->x(), this->y() + y, this->width(), this->height()));
	animation1->setEndValue(this->pos() + QPoint(0, y));
	//设置缓和曲线，QEasingCurve::OutBounce 为弹跳效果    
	animation1->setEasingCurve(QEasingCurve::OutBounce);
	//开始执行动画
	animation1->start();
}

void EndingPanel::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	p.drawPixmap(rect(), m_picture);

}
