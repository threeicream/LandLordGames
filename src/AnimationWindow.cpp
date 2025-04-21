#include "AnimationWindow.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

void AnimationWindow::showBetScore(int bet)
{
	//QString styleSheet; 
	
	if (bet == 1) {
		m_image.load(QString(":/LordGame/image/score1.png"));
		//styleSheet = QString("QWidget  {background-image: url(":/LordGame/image/score1.png"); }");
	}
	else if (bet == 2) {
		m_image.load(QString(":/LordGame/image/score2.png"));
		//styleSheet = QString("QWidget  {background-image: url(":/LordGame/image/score2.png"); }");
	}
	else  if (bet == 3) {
		m_image.load(QString(":/LordGame/image/score3.png"));
		//styleSheet = QString("QWidget  {background-image: url(":/LordGame/image/score3.png"); }");
	}
	//this->setStyleSheet(styleSheet);
	update();

	QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, m_image.width(), m_image.height(), m_image);
}

AnimationWindow::AnimationWindow(QWidget *parent)
	: QWidget(parent)
{
}

AnimationWindow::~AnimationWindow()
{
}
