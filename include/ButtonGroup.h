#pragma once

#include <QWidget>
#include "ui_ButtonGroup.h"

class ButtonGroup : public QWidget
{
	Q_OBJECT

public:
	ButtonGroup(QWidget *parent = nullptr);
	~ButtonGroup();
public:
	//按钮初始化
	void initButtons();
private:
	Ui::ButtonGroup ui;
};
