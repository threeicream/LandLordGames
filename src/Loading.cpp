#include "Loading.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include "gamePanel.h"

void Loading::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	p.drawPixmap(rect(), m_bk);
	p.drawPixmap(62, 417, m_progress.width(), m_progress.height(), m_progress);
}

Loading::Loading(QWidget *parent)
	: QWidget(parent)
{
	m_width = 3;
	m_bk.load(":/LordGame/image/loading.png");
	this->setFixedSize(m_bk.size());
	QPixmap pix(":/LordGame/image/progress.png");
	m_timer = new QTimer(this);

	// 去边框
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());
	// 背景透明
	setAttribute(Qt::WA_TranslucentBackground);

	connect(m_timer, &QTimer::timeout, this, [=]() {
		if (m_width > pix.width()) {
			m_timer->stop();
			m_timer->deleteLater();
			gamePanel* w = new gamePanel();
			w->show();
			close();
		}
		m_progress = pix.copy(0, 0, m_width, m_progress.height());
		m_width += 5;
		update();
		});
}

Loading::~Loading()
{
}

void Loading::startLoading()
{
	m_timer->start(15);
}
