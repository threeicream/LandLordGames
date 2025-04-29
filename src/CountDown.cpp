#include "CountDown.h"
#include <QTimer>
#include <QLabel>
#include <QPainter>
//#include <QGridLayout>

CountDown::CountDown(QWidget *parent)
	: QWidget(parent)
{
	m_timer = new QTimer(this);

	m_clock = new QLabel(this);
	m_clock->setFixedSize(70, 70);
	QString pix = QString("border-image: url(:/LordGame/image/clock.png);");
	m_clock->setStyleSheet(pix);
	m_clock->hide();

	//showCountDown();
	connect(m_timer, &QTimer::timeout, this, [&]() {
		--m_count;
		if (m_count < 10 && m_count >= 0) {
			qDebug() << m_count;
			m_clock->show();

			m_num = QPixmap(":/LordGame/image/number.png").copy(m_count * (30 + 10), 0, 30, 42).scaled(20, 30);
			// 创建 QIcon 并设置到 QLabel 上
			m_clock->setContentsMargins(0, 8, 3, 0); //设置边距
			m_clock->setPixmap(m_num); // 使用 setPixmap
			m_clock->setAlignment(Qt::AlignCenter);  // 居中显示

			if (m_count == 5) {
				emit notMuchTime();
			}
		}
		else if (m_count < 0) {
			m_clock->hide();
			m_num = QPixmap();
			m_timer->stop();
			qDebug() << "Emitting timeOut signal"; // 添加调试输出
			emit timeOut();
		}
		});
	//m_timerpix->move((width() - m_timerpix->width()) / 2, (height() - m_timerpix->height()) / 2 + 80);
	//m_timerpix->show();

	//m_time = new QLabel(QString("%1").arg(m_t--), this);
	//m_time->setFixedSize(20, 20);
	//QFont font("Helvetica", 12, QFont::Bold); // 使用字体名称和字号初始化
	//m_time->setFont(font);
	//m_time->move(m_timerpix->x() + (m_timerpix->width() - m_time->width()) / 2, m_timerpix->y() + (m_timerpix->height() - m_time->height()) / 2);
	//m_time->show();
}

CountDown::~CountDown()
{
}

void CountDown::showCountDown()
{
	m_count = 15;
	m_timer->start(1000);
}

void CountDown::stopCountDown()
{
	m_timer->stop();
	m_clock->hide();
	m_num = QPixmap();
}

//void CountDown::paintEvent(QPaintEvent* event)
//{
//	//QPainter p(this);
//	//p.drawPixmap(24, 24, m_num.width(), m_num.height(), m_num);
//}
