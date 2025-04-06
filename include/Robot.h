#pragma once

#include "Player.h"
#include <QObject>

class Robot : public Player
{
	Q_OBJECT
public:
	virtual void prepareCallLord() override;
	virtual void preparePlayHand() override;
public:
	using Player::Player;
	Robot(QObject* parent = nullptr);
	~Robot();
};
