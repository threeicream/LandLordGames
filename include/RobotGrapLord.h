#pragma once

#include <QThread>
#include <QObject>
class Robot;

class RobotGrapLord : public QThread
{
	Q_OBJECT
protected:
	void run();
public:
	RobotGrapLord(Robot* player, QObject* parent = nullptr);
	~RobotGrapLord();
private:
	Robot* m_player;
};

class robotGrapLord : public QObject
{
	Q_OBJECT
public:
	void workGrapLord();
	void workPlayHand();
public:
	robotGrapLord(Robot* player, QObject* parent = nullptr);
	~robotGrapLord();
private:
	Robot* m_player;
};
