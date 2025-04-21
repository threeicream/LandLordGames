#pragma once

#include <QThread>

class RobotPlayHand : public QThread
{
	Q_OBJECT

protected:
	void run()override;
public:
	RobotPlayHand(Robot* player , QObject* parent = nullptr);
	~RobotPlayHand();
private:
	Robot* m_player;
};
