#include "ButtonGroup.h"

ButtonGroup::ButtonGroup(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ButtonGroup::~ButtonGroup()
{
}

void ButtonGroup::initButtons()
{
	//开始游戏
	ui.startButton->setText("");
	ui.startButton->setImage(":/LordGame/image/start-1.png",":/LordGame/image/start-2.png",":/LordGame/image/start-3.png");
	//出牌
	ui.playCardButton->setText("");
	ui.playCardButton->setImage(":/LordGame/image/chupai_btn-1.png", ":/LordGame/image/chupai_btn-2.png", ":/LordGame/image/chupai_btn-3.png");
	//不要
	ui.passsButton->setText("");
	ui.passsButton->setImage(":/LordGame/image/chupai_pass_btn-1.png", ":/LordGame/image/chupai_pass_btn-1.png", ":/LordGame/image/chupai_pass_btn-1.png");
	ui.enterButton->setText("");
	ui.enterButton->setImage(":/LordGame/image/chupai_btn-1.png", ":/LordGame/image/chupai_btn-2.png", ":/LordGame/image/chupai_btn-3.png");
	//0分
	ui.zeroButton->setText("");
	ui.zeroButton->setImage(":/LordGame/image/buqiang-1.png", ":/LordGame/image/buqiang-2.png", ":/LordGame/image/buqiang-3.png");
	//1，2，3分
	ui.oneButton->setText("");
	ui.oneButton->setImage(":/LordGame/image/1fen-1.png", ":/LordGame/image/1fen-2.png", ":/LordGame/image/1fen-3.png");
	ui.twoButton->setText("");
	ui.twoButton->setImage(":/LordGame/image/2fen-1.png", ":/LordGame/image/2fen-2.png", ":/LordGame/image/2fen-3.png");
	ui.threeButton->setText("");
	ui.threeButton->setImage(":/LordGame/image/3fen-1.png", ":/LordGame/image/3fen-2.png", ":/LordGame/image/3fen-3.png");

	connect(ui.startButton, &QPushButton::clicked, this, &ButtonGroup::startGame);

	connect(ui.playCardButton, &QPushButton::clicked, this, &ButtonGroup::playHand);
	connect(ui.enterButton, &QPushButton::clicked, this, &ButtonGroup::playHand);

	connect(ui.passsButton, &QPushButton::clicked, this, &ButtonGroup::pass);

	connect(ui.zeroButton, &QPushButton::clicked, this, [=]() {emit betPoint(0); });
	connect(ui.oneButton, &QPushButton::clicked, this, [=]() {emit betPoint(1); });
	connect(ui.twoButton, &QPushButton::clicked, this, [=]() {emit betPoint(2); });
	connect(ui.threeButton, &QPushButton::clicked, this, [=]() {emit betPoint(3); });
}

void ButtonGroup::selectPanel(Panel type)
{
	ui.stackedWidget->setCurrentIndex(type);
}
