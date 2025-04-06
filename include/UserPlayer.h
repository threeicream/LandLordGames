#pragma once

#include "Player.h"
#include <QObject>

class UserPlayer : public Player
{
	Q_OBJECT
public:
	virtual void prepareCallLord() override;
	virtual void preparePlayHand() override;
public:
	using Player::Player;//继承基类构造函数
	UserPlayer(QObject *parent);
	~UserPlayer();
};
