#pragma once

#include <QWidget>
#include "ui_gamePanel.h"

class gamePanel : public QWidget
{
	Q_OBJECT

public:
	gamePanel(QWidget* parent = nullptr);
	~gamePanel();

private:
	Ui::gamePanel ui;
private:
	
};
