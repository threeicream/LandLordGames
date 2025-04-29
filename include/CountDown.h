#pragma once

#include <QWidget>
#include <QPixmap>
class QTimer;
class QPaintEvent;
class QLabel;

class CountDown : public QWidget
{
	Q_OBJECT

public:
	CountDown(QWidget* parent = nullptr);
	~CountDown();

	void showCountDown();
	void stopCountDown();
signals:
	//还剩五秒
	void notMuchTime();
	//时间到
	void timeOut();
protected:
	//virtual void paintEvent(QPaintEvent* event)override;
private:
	QLabel* m_clock;
	QPixmap m_num;
	QTimer* m_timer = nullptr;
	int m_count;
};
