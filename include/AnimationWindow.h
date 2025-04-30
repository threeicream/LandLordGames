#pragma once

#include <QWidget>
#include <QPixmap>
class QTimer;
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
	void showPlane();
	//显示炸弹
	void showBomb();
	//显示王炸
	void showJokerBomb();
protected:
	void paintEvent(QPaintEvent* event);
public:
	AnimationWindow(QWidget* parent = nullptr);
	~AnimationWindow();
private:
	QPixmap m_image;
	int m_bomb;
	int m_x;
};
