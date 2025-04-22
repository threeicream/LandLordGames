#include "RobotGrapLord.h"
#include "Robot.h"
#include <QTimer>

void RobotGrapLord::run()
{
	//考虑叫地主
	msleep(2000);
	//sleep(2);
	m_player->thinkCallLorad();
}

RobotGrapLord::RobotGrapLord(Robot* player , QObject* parent)
	: QThread(parent)
{
	m_player = player;
}

RobotGrapLord::~RobotGrapLord()
{
}

void robotGrapLord::workGrapLord()
{
	//考虑叫地主
	QTimer::singleShot(2000, this, [&]() {m_player->thinkCallLorad(); });
	
}

void robotGrapLord::workPlayHand()
{
	QTimer::singleShot(2000, this, [&]() {m_player->thinkPlayHand(); });
}

robotGrapLord::robotGrapLord(Robot* player, QObject* parent)
{
	m_player = player;
}

robotGrapLord::~robotGrapLord()
{
}
