#include "RobotGrapLord.h"
#include "Robot.h"

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
