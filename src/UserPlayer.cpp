#include "UserPlayer.h"

void UserPlayer::prepareCallLord()
{
}

void UserPlayer::preparePlayHand()
{
}

UserPlayer::UserPlayer(QObject *parent)
	: Player(parent)
{
	this->m_tyep = Player::USER;
}

UserPlayer::~UserPlayer()
{
}
