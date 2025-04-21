#pragma once

#include <QThread>

class RobotPlayHand : public QThread
{
	Q_OBJECT

public:
	RobotPlayHand(QObject *parent);
	~RobotPlayHand();
};
