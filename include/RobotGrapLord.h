#pragma once

#include <QThread>
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
