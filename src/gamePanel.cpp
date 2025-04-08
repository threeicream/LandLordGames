#include "gamePanel.h"
#include "myButton.h"
#include <QVBoxLayout>

gamePanel::gamePanel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

gamePanel::~gamePanel()
{
}
