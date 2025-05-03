#pragma once

#include <QObject>

class BgmControl : public QObject
{
	Q_OBJECT

public:
	BgmControl(QObject* parent = nullptr);
	~BgmControl();
};
