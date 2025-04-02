#pragma once

#include <QObject>

class Player : public QObject
{
	Q_OBJECT

public:
	Player(QObject* parent = nullptr);
	~Player();
};
