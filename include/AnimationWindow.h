#pragma once

#include <QWidget>
#include <QPixmap>
class QPaintEvent;

class AnimationWindow : public QWidget
{
	Q_OBJECT
public:
	enum Type { Sequence, Pair };
	//显示下注分数
	void showBetScore(int bet);
	//显示顺子和连对
	void showSequenceAndPair(Type type);
	//显示飞机

	//显示炸弹

	//显示王炸
protected:
	void paintEvent(QPaintEvent* event);
public:
	AnimationWindow(QWidget* parent = nullptr);
	~AnimationWindow();
private:
	QPixmap m_image;
};
