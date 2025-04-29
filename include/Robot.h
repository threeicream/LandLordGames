#pragma once

#include "Player.h"
#include <QObject>
class QThread;
class Strategy;

class Robot : public Player
{
	Q_OBJECT
public:
	virtual void prepareCallLord() override;
	virtual void preparePlayHand() override;

	//考虑叫地主
	void thinkCallLorad();

	//考虑出牌
	void thinkPlayHand();
	void thinkPlayHand_api();
public:
	using Player::Player;//继承基类构造函数
	Robot(QObject* parent = nullptr);
	~Robot();
private:
	//Strategy* str = nullptr;
};
