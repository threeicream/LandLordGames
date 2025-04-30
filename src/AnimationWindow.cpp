#include "AnimationWindow.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

void AnimationWindow::showBetScore(int bet)
{	
	m_x = 0;
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
	m_x = 0;
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

void AnimationWindow::showPlane()
{
	m_x = width();
	m_image.load(":/LordGame/image/plane_1.png");
	setFixedHeight(m_image.height());
	update();

	m_bomb = 1;
	int step = width() / 5;
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [=]() {
		
		static int dist = 0;
		dist += 5;
		if (dist >= step) {
			++m_bomb;
			dist = 0;
			m_image.load(QString(":/LordGame/image/plane_%1.png").arg(m_bomb % 5 + 1));
		}
		m_x -= 5;//飞机向左飞行
		update();
		if (m_x <= -110) {
			dist = m_bomb = 0;
			hide();
			timer->stop();
			timer->deleteLater();
		}
		
		update();
		});
	timer->start(15);//15ms启动一次
}

void AnimationWindow::showBomb()
{
	m_bomb = 0;
	m_x = 0;
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [=]() {
		if (m_bomb > 12) {
			m_bomb = 12;
			hide();
			timer->stop();
			timer->deleteLater();
		}
		m_image.load(QString(":/LordGame/image/bomb_%1.png").arg(++m_bomb));
		update();
		});
	timer->start(60);//60ms启动一次
}

void AnimationWindow::showJokerBomb()
{
	m_bomb = 0;
	m_x = 0;
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [=]() {
		if (m_bomb > 8) {
			m_bomb = 8;
			hide();
			timer->stop();
			timer->deleteLater();
		}
		m_image.load(QString(":/LordGame/image/joker_bomb_%1.png").arg(++m_bomb));
		update();
		});
	timer->start(60);//60ms启动一次
}

void AnimationWindow::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(m_x, 0, m_image.width(), m_image.height(), m_image);

	QWidget::paintEvent(event);
}

AnimationWindow::AnimationWindow(QWidget *parent)
	: QWidget(parent)
{
	m_bomb = 0;
	m_x = 0;
}

AnimationWindow::~AnimationWindow()
{
}
