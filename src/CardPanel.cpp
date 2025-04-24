#include "CardPanel.h"
#include <QPainter>
#include <QMouseEvent>

CardPanel::CardPanel(QWidget *parent)
	: QWidget(parent),m_isFront(true)
{
}

CardPanel::~CardPanel()
{
}

void CardPanel::setImage(QPixmap& front, QPixmap& back)
{
	m_front = front;
	m_back = back;

	this->setFixedSize(m_front.size());

	this->update();//请求重绘，但不立即执行
	//this->repaint();//请求重绘，立即执行
}

QPixmap CardPanel::getFrontImage() const
{
	return m_front;
}

void CardPanel::setFrontSide(bool flag)
{
	m_isFront = flag;
}

bool CardPanel::isFrontSide()
{
	return m_isFront;
}

void CardPanel::setSelected(bool flag)
{
	m_isSelected = flag;
}

bool CardPanel::isSelected()
{
	return m_isSelected;
}

void CardPanel::setCard(Card& card)
{
	m_card = card;
}

Card CardPanel::getCard()
{
	return m_card;
}

void CardPanel::setOwner(Player* player)
{
	m_owner = player;
}

Player* CardPanel::getOwner()
{
	return m_owner;
}

void CardPanel::clicked()
{
	emit cardSelected(Qt::LeftButton);
}

void CardPanel::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	if (m_isFront) {//绘制正面
		painter.drawPixmap(this->rect(), m_front);
	}
	else {//绘制背面
		painter.drawPixmap(this->rect(), m_back);
	}

	QWidget::paintEvent(event);
}

void CardPanel::mousePressEvent(QMouseEvent* event)
{
	emit cardSelected(event->button());
}
