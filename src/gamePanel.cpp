#include "gamePanel.h"
#include "ButtonGroup.h"
#include "GameControl.h"
#include "Robot.h"
#include "UserPlayer.h"
#include "CardPanel.h"
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QPainter>
#include <QPixmap>
#include <QPoint>

gamePanel::gamePanel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

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
	CardMapInit();
	//6.初始化游戏中的按钮组
	buttonGroupInit();
	//7.初始化玩家在窗口的上下文环境
	PlayerContextInit();
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

void gamePanel::CardMapInit()
{
	//1.加载大图
	QPixmap pixmap(":/LordGame/image/card.png");
	//2.计算每张图片大小
	m_cardSize.setWidth(pixmap.width() / 13);
	m_cardSize.setHeight(pixmap.height() / 5);
	//3.背景图
	m_cardBackImg = pixmap.copy(m_cardSize.width() * 2, m_cardSize.height() * 4, m_cardSize.width(), m_cardSize.height());
	//正常花色
	for (int i = 0, suit = Card::Suit_Begin + 1; suit < Card::Suit_End; ++suit, ++i) {
		for (int j = 0, pt = Card::Card_Begin + 1; pt < Card::Card_SJ; ++pt, ++j) {
			Card card(static_cast<Card::CardPoint>(pt), static_cast<Card::CardSuit>(suit));
			cropImg(pixmap, m_cardSize.width() * j, m_cardSize.height() * i, card);
		}
	}
	//大小王
	Card SJcard(Card::Card_SJ, Card::Suit_Begin);
	Card BJcard(Card::Card_BJ, Card::Suit_Begin);
	cropImg(pixmap, m_cardSize.width() * 0, m_cardSize.height() * 4, SJcard);
	cropImg(pixmap, m_cardSize.width() * 1, m_cardSize.height() * 4, BJcard);
}

void gamePanel::cropImg(QPixmap& pix, int x, int y, Card& card)
{
	CardPanel* cardpanel = new CardPanel(this);
	cardpanel->hide();//隐藏
	QPixmap cardFrontImg = pix.copy(x, y, m_cardSize.width(), m_cardSize.height());
	cardpanel->setCard(card);
	cardpanel->setImage(cardFrontImg, m_cardBackImg);
	m_cardMap[card] = cardpanel;//需要card类定义<重载运算符
	//m_cardMap.insert(card, cardpanel);
}

void gamePanel::buttonGroupInit()
{
	ui.buttonGroup->initButtons();
	ui.buttonGroup->selectPanel(ButtonGroup::START);

	connect(ui.buttonGroup, &ButtonGroup::startGame, this, [](){});
	connect(ui.buttonGroup, &ButtonGroup::playHand, this, []() {});
	connect(ui.buttonGroup, &ButtonGroup::pass, this, []() {});
	connect(ui.buttonGroup, &ButtonGroup::betPoint, this, []() {});
}

void gamePanel::PlayerContextInit()
{
	//1.放置玩家扑克牌的区域
	//2.玩家出牌的区域
	//3.玩家头像显示的位置
	QRect cardRect[] = {
		//x,y,width,height
		QRect(260,150,100,100) ,//左侧机器人
		QRect(rect().right() - 360,150,100,100) ,//右侧机器人
		QRect(150,rect().bottom() - 290,this->width() - 300,100) //当前玩家
	};
	QRect playHandRect[] = {
		//x,y,width,height
		QRect(90,130,100,this->height() - 200) ,//左侧机器人
		QRect(rect().right() - 190,130,100,this->height() - 200) ,//右侧机器人
		QRect(250,rect().bottom() - 120,this->width() - 500,100) //当前玩家
	};
	QPoint roleImgPos[] = {
		//x,y,width,height
		QPoint(cardRect[0].left() - 80,cardRect[0].height() / 2 + 20) ,//左侧机器人
		QPoint(cardRect[1].right() + 10,cardRect[0].height() / 2 + 20) ,//右侧机器人
		QPoint(cardRect[2].right() - 10,cardRect[0].top() - 10) //当前玩家
	};
}

void gamePanel::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), m_backGroundPixmap);
}
