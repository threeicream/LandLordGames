#include "myButton.h"
#include <QPropertyAnimation>
#include <qDebug>
#include <QMouseEvent>
#include <QPainter>

void myButton::zoom1()
{
	//创建动画对象
	QPropertyAnimation* animation1 = new QPropertyAnimation(this, "geometry");
	//设置时间间隔，单位毫秒
	animation1->setDuration(200);
	//创建起始位置
	animation1->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
	//创建结束位置
	animation1->setEndValue(QRect(this->x(), this->y() + 10, this->width(), this->height()));
	//设置缓和曲线，QEasingCurve::OutBounce 为弹跳效果    
	animation1->setEasingCurve(QEasingCurve::OutBounce);
	//开始执行动画
	animation1->start();
}

void myButton::zoom2()
{
	QPropertyAnimation* animation1 = new QPropertyAnimation(this, "geometry");
	animation1->setDuration(200);
	animation1->setStartValue(QRect(this->x(), this->y() + 10, this->width(), this->height()));
	animation1->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));
	animation1->setEasingCurve(QEasingCurve::OutBounce);
	animation1->start();
}

void myButton::setImage(QString normalImgPath, QString pressedImgPath, QString hoverImgPath)
{
	m_normalImgPath = normalImgPath;
	m_pressedImgPath = pressedImgPath;
	m_hoverImgPath = hoverImgPath;

	// 创建样式表字符串
	QString styleSheet = QString("QPushButton {"
		"    border-image: url(%1);"
		"    border: none;" // 移除边框，非常重要！
		"}"
		"QPushButton:hover {"
		"    border-image: url(%2);"
		"}"
		"QPushButton:pressed {"
		"    border-image: url(%3);"
		"}").arg(m_normalImgPath, m_hoverImgPath, m_pressedImgPath);
	setStyleSheet(styleSheet);

	//m_pixmap.load(m_normalImgPath);
}

myButton::myButton(QWidget *parent)
	: QPushButton(parent)
{
}

myButton::~myButton()
{
}

//void myButton::enterEvent(QEnterEvent* event)
//{
//	m_pixmap.load(m_hoverImgPath);
//	update();
//	QPushButton::enterEvent(event);
//}
//
//void myButton::leaveEvent(QEvent* event)
//{
//	m_pixmap.load(m_normalImgPath);
//	update();
//	QPushButton::leaveEvent(event);
//}
//
//void myButton::mousePressEvent(QMouseEvent* event)
//{
//	if (event->button() == Qt::LeftButton) {
//		m_pixmap.load(m_pressedImgPath);
//		update();
//	}
//	QPushButton::mousePressEvent(event);
//}
//
//void myButton::mouseReleaseEvent(QMouseEvent* event)
//{
//	if (event->button() == Qt::LeftButton) {
//		m_pixmap.load(m_normalImgPath);
//		update();
//	}
//	QPushButton::mouseReleaseEvent(event);
//}
//
//void myButton::paintEvent(QPaintEvent* event)
//{
//	QPainter P(this);
//	P.drawPixmap(rect(),m_pixmap);
//	//QPushButton::paintEvent(event);
//}
