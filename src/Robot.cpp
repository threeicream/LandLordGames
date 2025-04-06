#include "Robot.h"

void Robot::prepareCallLord()
{
}

void Robot::preparePlayHand()
{
}

Robot::Robot(QObject *parent)
	: Player(parent)
{
	this->m_tyep = Player::ROBOT;
}

Robot::~Robot()
{
}
