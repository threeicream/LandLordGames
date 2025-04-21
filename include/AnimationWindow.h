#pragma once

#include <QWidget>
#include <QPixmap>
class QPaintEvent;

class AnimationWindow : public QWidget
{
	Q_OBJECT
public:
	//显示下注分数
	void showBetScore(int bet);
protected:
	void paintEvent(QPaintEvent* event);
public:
	AnimationWindow(QWidget* parent = nullptr);
	~AnimationWindow();
private:
	QPixmap m_image;
};
