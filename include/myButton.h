#pragma once

#include <QPushButton>
#include <QPixmap>

class myButton : public QPushButton
{
	Q_OBJECT
public:
	void zoom1();//向下跳动
	void zoom2();//向上跳动

	void setImage(QString normalImgPath, QString pressedImgPath, QString hoverImgPath);
public:
	myButton(QWidget *parent);
	~myButton();
protected://使用QPixmap进行设置
	//virtual void enterEvent(QEnterEvent* event)override;//鼠标进入
	//virtual void leaveEvent(QEvent* event)override;//鼠标离开
	//virtual void mousePressEvent(QMouseEvent* event)override;
	//virtual void mouseReleaseEvent(QMouseEvent* event)override;
	//virtual void paintEvent(QPaintEvent* event)override;
private:
	QString m_normalImgPath;//默认显示图片路径
	QString m_pressedImgPath;//按下显示图片路径
	QString m_hoverImgPath;//悬停显示图片路径
	//QPixmap m_pixmap;
};
