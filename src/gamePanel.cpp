#include "gamePanel.h"
#include "ButtonGroup.h"
#include "GameControl.h"
#include "Robot.h"
#include "UserPlayer.h"
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QPainter>

gamePanel::gamePanel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.buttonGroup->initButtons();
	ui.buttonGroup->selectPanel(ButtonGroup::START);

	//1.背景图
	int backGround = QRandomGenerator::global()->bounded(1, 11);
	QString backGroundPath = QString(":/LordGame/image/background-%1.png").arg(backGround);
	m_backGroundPixmap.load(backGroundPath);
	//QString styleSheet = QString("#gamePanel  {background-image: url(%1); }").arg(backGroundPath);
	//this->setStyleSheet(styleSheet);
	//2.窗口的标题大小
	this->setWindowTitle("欢乐斗地主");
	this->setFixedSize(1000, 650);
	//3.游戏控制类实例化
	gameControlInit();
	//4.玩家得分(更新)
	UpdateScorePanel();
	//5.扑克牌的初始化（切割游戏图片）

	//6.初始化游戏中的按钮组

	//7.初始化玩家在窗口的上下文环境
}

gamePanel::~gamePanel()
{
}

void gamePanel::gameControlInit()
{
	m_gameCtl = new GameControl(this);
	m_gameCtl->playerInit();
	//得到三个玩家的实例对象
	m_playerList.append(m_gameCtl->getUserPlayer());
	m_playerList.append(m_gameCtl->getLeftRobot());
	m_playerList.append(m_gameCtl->getRightRobot());
}

void gamePanel::UpdateScorePanel()
{
	ui.scorePanel->setScore(
		m_playerList[0]->getScore(), 
		m_playerList[1]->getScore(), 
		m_playerList[2]->getScore());
}

void gamePanel::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), m_backGroundPixmap);
}
