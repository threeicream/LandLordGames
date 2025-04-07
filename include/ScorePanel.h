#pragma once

#include <QWidget>
#include "ui_ScorePanel.h"

class ScorePanel : public QWidget
{
	Q_OBJECT

public:
	ScorePanel(QWidget* parent = nullptr);
	~ScorePanel();

private:
	Ui::ScorePanel ui;
};
