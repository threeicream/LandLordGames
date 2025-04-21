#include "RobotPlayHand.h"
#include "Robot.h"

void RobotPlayHand::run()
{
	msleep(2000);
	m_player->thinkPlayHand();
}

RobotPlayHand::RobotPlayHand(Robot* player, QObject *parent)
	: QThread(parent)
{
	m_player = player;
}

RobotPlayHand::~RobotPlayHand()
{
}
