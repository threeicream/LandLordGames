#include "AnimationWindow.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

void AnimationWindow::showBetScore(int bet)
{	
	if (bet == 1) {
		m_image.load(QString(":/LordGame/image/score1.png"));
	}
	else if (bet == 2) {
		m_image.load(QString(":/LordGame/image/score2.png"));
	}
	else  if (bet == 3) {
		m_image.load(QString(":/LordGame/image/score3.png"));
	}
	update();

	QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::showSequenceAndPair(Type type)
{
	switch (type)
	{
	case AnimationWindow::Sequence:
		m_image.load(QString(":/LordGame/image/shunzi.png"));
		break;
	case AnimationWindow::Pair:
		m_image.load(QString(":/LordGame/image/liandui.png"));
		break;
	default:
		break;
	}
	update();

	QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, m_image.width(), m_image.height(), m_image);

	QWidget::paintEvent(event);
}

AnimationWindow::AnimationWindow(QWidget *parent)
	: QWidget(parent)
{
}

AnimationWindow::~AnimationWindow()
{
}
