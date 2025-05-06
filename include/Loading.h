#pragma once

#include <QWidget>
#include <QPixmap>
class QPaintEvent;
class QTimer;

class Loading : public QWidget
{
	Q_OBJECT
protected:
	void paintEvent(QPaintEvent* event);
public:
	Loading(QWidget *parent = nullptr);
	~Loading();

	void startLoading();
private:
	QPixmap m_bk;
	QPixmap m_progress;
	QTimer* m_timer = nullptr;
	int m_width;
};
