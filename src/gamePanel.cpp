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
#include <QLabel>

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
	//8.扑克牌场景初始化
	GameSceneInit();
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

	connect(ui.buttonGroup, &ButtonGroup::startGame, this, [=](){
		//界面初始化
		ui.buttonGroup->selectPanel(ButtonGroup::EMPTY);
		////清空得分（可以不写，重复了）
		//m_gameCtl->clearPlayerScore();
		////更新得分
		//UpdateScorePanel();

		});
	connect(ui.buttonGroup, &ButtonGroup::playHand, this, [=]() {});
	connect(ui.buttonGroup, &ButtonGroup::pass, this, [=]() {});
	connect(ui.buttonGroup, &ButtonGroup::betPoint, this, [=]() {});
}

void gamePanel::PlayerContextInit()
{
	//1.放置玩家扑克牌的区域
	QRect cardRect[] = {
		//x,y,width,height
		QRect(260,150,100,100) ,//左侧机器人
		QRect(rect().right() - 360,150,100,100) ,//右侧机器人
		QRect(150,rect().bottom() - 290,this->width() - 300,100) //当前玩家
	};
	//2.玩家出牌的区域
	QRect playHandRect[] = {
		//x,y,width,height
		QRect(90,130,100,this->height() - 200) ,//左侧机器人
		QRect(rect().right() - 190,130,100,this->height() - 200) ,//右侧机器人
		QRect(250,rect().bottom() - 120,this->width() - 500,100) //当前玩家
	};
	//3.玩家头像显示的位置
	QPoint roleImgPos[] = {
		//x,y,width,height
		QPoint(cardRect[0].left() - 80,cardRect[0].height() / 2 + 20) ,//左侧机器人
		QPoint(cardRect[1].right() + 10,cardRect[0].height() / 2 + 20) ,//右侧机器人
		QPoint(cardRect[2].right() - 10,cardRect[0].top() - 10) //当前玩家
	};

	//循环
	int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());//用于查找 QVector 中指定值的第一个匹配项的索引
	for (int i = 0; i < m_playerList.size(); ++i) {
		PlayerContext context;
		context.align = (i == index ? gamePanel::HOR : gamePanel::VER);
		context.isFrontSide = (i == index ? true : false);
		context.cardRect = cardRect[i];
		context.PlayHandRect = playHandRect[i];
		//提示信息
		context.info = new QLabel(this);
		context.info->resize(160, 98);
		context.info->hide();
		//显示到出牌区域的中心位置
		QRect rect = playHandRect[i];
		QPoint pt(rect.left() + (rect.width() - context.info->width()) / 2,//套中套矩形，画下图更好理解
				  rect.top() + (rect.height() - context.info->height()) / 2);
		context.info->move(pt);
		//玩家的头像
		context.roleImg = new QLabel(this);
		context.roleImg->resize(84, 120);
		context.roleImg->hide();
		context.roleImg->move(roleImgPos[i]);
		//玩家打出的牌（为空）
		
		m_contextMap[m_playerList[i]] = context;
	}
}

void gamePanel::GameSceneInit()
{
	//1. 发牌区的扑克牌
	m_baseCard = new CardPanel(this);
	m_baseCard->setImage(m_cardBackImg, m_cardBackImg);
	//2. 发牌过程中移动的扑克牌
	m_moveCard = new CardPanel(this);
	//3. 最后的三张底牌（用于窗口显示）
	for (int i = 0; i < 3; ++i) {
		CardPanel* panel = new CardPanel(this);
		panel->setImage(m_cardBackImg, m_cardBackImg);
		m_last3Card.append(panel);
		panel->hide();
	}
	//卡牌的位置
	m_baseCardPos = QPoint((width() - m_cardSize.width()) / 2,
						   (height() - m_cardSize.height()) / 2 - 100);
	m_baseCard->move(m_baseCardPos);
	m_moveCard->move(m_baseCardPos);

	int base = (width() - 3 * m_cardSize.width() - 2 * 10) / 2;//3张牌，2个间隙
	for (int i = 0; i < 3; ++i) {
		m_last3Card[i]->move(base + (m_cardSize.width() + 10) * i, 20);
	}
}

void gamePanel::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), m_backGroundPixmap);
}
