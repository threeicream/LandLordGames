#include "gamePanel.h"
#include "ButtonGroup.h"
#include "Robot.h"
#include "UserPlayer.h"
#include "CardPanel.h"
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include "AnimationWindow.h"
#include "PlayHand.h"

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
	//9.定时器实例化
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &gamePanel::onDispatchCard);

	m_animation = new AnimationWindow(this);

}

gamePanel::~gamePanel()
{
}

void gamePanel::gameControlInit()
{
	m_gameCtl = new GameControl(this);
	m_gameCtl->playerInit();
	//得到三个玩家的实例对象
	m_playerList.append(m_gameCtl->getLeftRobot());
	m_playerList.append(m_gameCtl->getRightRobot());
	m_playerList.append(m_gameCtl->getUserPlayer());

	connect(m_gameCtl, &GameControl::playerStatusChanged, this, &gamePanel::onPlayerStatusChanged);
	connect(m_gameCtl, &GameControl::notifyGrabLordBet, this, &gamePanel::onGrabLordBet);
	connect(m_gameCtl, &GameControl::gameStatusChanged, this, &gamePanel::gameStatusProcess);
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
			//Card card(static_cast<Card::CardPoint>(pt), static_cast<Card::CardSuit>(suit));
			Card card((Card::CardPoint)pt, (Card::CardSuit)suit);
			cropImg(pixmap, m_cardSize.width() * j, m_cardSize.height() * i, card);
		}
	}
	//大小王
	Card c;
	c.setPoint(Card::Card_SJ);
	c.setSuit(Card::Suit_Begin);
	cropImg(pixmap, 0, 4 * m_cardSize.height(), c);

	c.setPoint(Card::Card_BJ);
	cropImg(pixmap, m_cardSize.width(), 4 * m_cardSize.height(), c);
}

void gamePanel::cropImg(QPixmap& pix, int x, int y, Card& card)
{
	QPixmap cardFrontImg = pix.copy(x, y, m_cardSize.width(), m_cardSize.height());
	CardPanel* cardpanel = new CardPanel(this);
	cardpanel->setImage(cardFrontImg, m_cardBackImg);
	cardpanel->setCard(card);
	cardpanel->hide();//隐藏
	m_cardMap[card] = cardpanel;//需要card类定义<重载运算符
	//m_cardMap.insert(card, cardpanel);
	//connect(cardpanel, &CardPanel::cardSelected, this, &GamePanel::onCardSelected);
}

void gamePanel::buttonGroupInit()
{
	ui.buttonGroup->initButtons();
	ui.buttonGroup->selectPanel(ButtonGroup::START);

	connect(ui.buttonGroup, &ButtonGroup::startGame, this, [=](){
		//界面初始化
		ui.buttonGroup->selectPanel(ButtonGroup::EMPTY);
		//清空得分（可以不写，重复了）
		m_gameCtl->clearPlayerScore();
		//更新得分（可以不写，重复了）
		UpdateScorePanel();
		//编写游戏状态->发牌
		gameStatusProcess(GameControl::GameStatus::DISPATCHCARD);

		});
	connect(ui.buttonGroup, &ButtonGroup::playHand, this, [=]() {});
	connect(ui.buttonGroup, &ButtonGroup::pass, this, [=]() {});
	connect(ui.buttonGroup, &ButtonGroup::betPoint, this, [=](int bet) {//玩家下注
		m_gameCtl->getUserPlayer()->grabLordBet(bet);
		ui.buttonGroup->selectPanel(ButtonGroup::EMPTY);
		});
}

void gamePanel::PlayerContextInit()
{
	//1.放置玩家扑克牌的区域
	QRect cardRect[] = {
		//x,y,width,height
		QRect(90, 130, 100, height() - 200),                    // 左侧机器人
		QRect(rect().right() - 190, 130, 100, height() - 200),  // 右侧机器人
		QRect(250, rect().bottom() - 120, width() - 500, 100)   // 当前玩家
	};
	//2.玩家出牌的区域
	QRect playHandRect[] = {
		//x,y,width,height
		QRect(260, 150, 100, 100),                              // 左侧机器人
		QRect(rect().right() - 360, 150, 100, 100),             // 右侧机器人
		QRect(150, rect().bottom() - 290, width() - 300, 105)   // 当前玩家
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
	m_moveCard->setImage(m_cardBackImg, m_cardBackImg);
	//3. 最后的三张底牌（用于窗口显示）
	for (int i = 0; i < 3; ++i) {
		CardPanel* panel = new CardPanel(this);
		panel->setImage(m_cardBackImg, m_cardBackImg);
		m_last3Card.append(panel);
		panel->hide();
	}
	//卡牌的位置
	m_baseCardPos = QPoint((width() - m_cardSize.width()) / 2,
						   height() / 2 - 100);
	m_baseCard->move(m_baseCardPos);
	m_moveCard->move(m_baseCardPos);

	int base = (width() - 3 * m_cardSize.width() - 2 * 10) / 2;//3张牌，2个间隙
	for (int i = 0; i < 3; ++i) {
		m_last3Card[i]->move(base + (m_cardSize.width() + 10) * i, 20);
	}
}

void gamePanel::gameStatusProcess(GameControl::GameStatus status)
{
	//记录游戏状态
	m_gameStatus = status;
	CardLsit last3Card; // 在 switch 语句之前声明 last3Card
	//处理游戏状态
	switch (m_gameStatus)
	{
	case GameControl::DISPATCHCARD://发牌
		startDispatchCard();
		break;
	case GameControl::CALLINGLORD://叫地主
		//取出底牌数据
		last3Card = m_gameCtl->getSurplusCards().toCardList();
		//给底牌窗口设置图片
		for (int i = 0; i < last3Card.size(); ++i) {
			QPixmap front = m_cardMap[last3Card[i]]->getFrontImage();
			m_last3Card[i]->setImage(front, m_cardBackImg);
			m_last3Card[i]->hide();
		}
		//叫地主
		m_gameCtl->startLordCard();
		break;
	case GameControl::PLAYINGHAND:
		break;
	default:
		break;
	}
}

void gamePanel::startDispatchCard()
{
	//重置每张牌的属性
	for (auto it = m_cardMap.begin(); it != m_cardMap.end(); ++it) {
		it.value()->setSelected(false);
		it.value()->setFrontSide(true);
		it.value()->hide();
	}
	//隐藏最后三张底牌
	for (int i = 0; i < m_last3Card.size(); ++i) {
		m_last3Card[i]->hide();
	}
	//重置所有玩家的窗口上下文信息
	int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
	for (int i = 0; i < m_playerList.size(); ++i) {
		m_contextMap[m_playerList[i]].lastCards.clear();
		m_contextMap[m_playerList[i]].info->hide();
		m_contextMap[m_playerList[i]].roleImg->hide();
		m_contextMap[m_playerList[i]].isFrontSide = (i == index ? true : false);
		//m_contextMap[m_playerList[i]].align = (i == index ? CardAlign::HOR : CardAlign::VER);
		//玩家头像位置、卡牌显示区域、出牌区域、对齐方式不需要修改，已经固定
	}
	//重置所有玩家手牌数据
	m_gameCtl->resetCardData();
	//显示底牌
	m_baseCard->show();
	//隐藏按钮面板
	ui.buttonGroup->selectPanel(ButtonGroup::EMPTY);
	//启动定时器
	m_timer->start(10);//10ms触发一次
	//播放背景音乐
}

void gamePanel::onDispatchCard()
{
	/*int x = 0, y = 0;//测试用
	int i = 0, j = 0;

	for (auto it = m_cardMap.begin(); it != m_cardMap.end(); ++it) {
		it.value()->show();
		it.value()->move(QPoint(x + i * m_baseCard->width() , y + j * m_baseCard->height()));
		++i;
		if (i == 13) {
			i = 0;
			++j;
		}
	}*/
	//记录扑克牌的位置
	static int curMovePos = 0;
	//当前玩家
	Player* curPlayer = m_gameCtl->getCurrentPlayer();
	if (curMovePos >= 100) {
		//给玩家发一张牌
		Card card = m_gameCtl->takeOneCard();
		curPlayer->storeDispatchCard(card);
		Cards cards(card);
		disposCard(curPlayer, cards);
		//切换玩家
		m_gameCtl->setCurrentPlayer(curPlayer->getNextPlayer());
		curMovePos = 0;
		//发牌动画
		cardMoveStep(curPlayer, curMovePos);
		//判断牌是否发完了
		if (m_gameCtl->getSurplusCards().cardCount() == 3) {
			//终止定时器
			m_timer->stop();
			//切换游戏状态
			gameStatusProcess(GameControl::CALLINGLORD);

			return;
		}
	}
	//移动扑克牌
	cardMoveStep(curPlayer, curMovePos);
	curMovePos += 15;
}

void gamePanel::cardMoveStep(Player* player, int curPos)
{
	//得到每个玩家的扑克牌展示区域
	QRect cardRect = m_contextMap[player].cardRect;
	//每个玩家的单元步长
	const int unit[] = {
	(m_baseCardPos.x() - cardRect.right()) / 100,//左侧玩家
	(cardRect.left() - m_baseCardPos.x()) / 100,//右侧玩家
	(cardRect.top() - m_baseCardPos.y()) / 100//下侧玩家
	};
	//每次窗口移动时每个玩家对应的牌的实时坐标位置
	const QPoint pos[] = {
	QPoint(m_baseCardPos.x() - curPos * unit[0],m_baseCardPos.y()),
	QPoint(m_baseCardPos.x() + curPos * unit[1],m_baseCardPos.y()),
	QPoint(m_baseCardPos.x(),m_baseCardPos.y() + curPos * unit[2])
	};

	//移动扑克牌窗口
	int index = m_playerList.indexOf(player);
	m_moveCard->move(pos[index]);

	//临界状态处理
	if (curPos == 0) {
		m_moveCard->show();
	}
	if (curPos >= 100) {
		m_moveCard->hide();
	}
}

void gamePanel::zoomY(int y)
{
	////创建动画对象
	//QPropertyAnimation* animation1 = new QPropertyAnimation(m_moveCard, "geometry");
	////设置时间间隔，单位毫秒
	//animation1->setDuration(10);
	////创建起始位置
	//animation1->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
	////创建结束位置
	//animation1->setEndValue(QRect(this->x(), this->y() + y, this->width(), this->height()));
	////设置缓和曲线，QEasingCurve::OutBounce 为弹跳效果    
	//animation1->setEasingCurve(QEasingCurve::OutBounce);
	////开始执行动画
	//animation1->start();
}

void gamePanel::disposCard(Player* player, Cards& cards)
{
	CardLsit list = cards.toCardList();
	for (int i = 0; i < list.size(); ++i) {
		CardPanel* panel = m_cardMap[list[i]];
		panel->setOwner(player);
		//player->setca
	}
	//更新扑克牌在窗口的显示
	updatePlayerCards(player);

}

void gamePanel::updatePlayerCards(Player* player)
{
	Cards cards = player->getCards();
	CardLsit list = cards.toCardList();
	//取出展示扑克牌的区域
	int cardSpace = 20;
	QRect cardsRect = m_contextMap[player].cardRect;
	for (int i = 0; i < list.size(); ++i) {
		CardPanel* panel = m_cardMap[list[i]];
		panel->show();
		panel->raise();//让其位于最上层
		panel->setFrontSide(m_contextMap[player].isFrontSide);
		//水平or垂直显示
		if (m_contextMap[player].align==HOR) {
			int leftx = cardsRect.left() + (cardsRect.width() - (list.size() - 1) * cardSpace - panel->width()) / 2;
			int topy = cardsRect.top() + (cardsRect.height() - panel->height()) / 2;
			if (panel->isSelected()) {
				topy -= 10;
			}
			panel->move(leftx + cardSpace * i, topy);
		}
		else {
			int leftx = cardsRect.left() + (cardsRect.width() - panel->width()) / 2;
			int topy = cardsRect.top() + (cardsRect.height() - (list.size() - 1) * cardSpace - panel->height()) / 2;
			panel->move(leftx, topy + cardSpace * i);
		}
	}
}

void gamePanel::onDisposePlayHand(Player* player, Cards& cards)
{
	//1.隐藏上一轮打出的牌
	hidePlayerDropCards(player);
	//存储玩家打出的牌
	auto it = m_contextMap.find(player);
	it->lastCards = cards;
	//2.根据牌型播放音效
	PlayHand hand(cards);
	PlayHand::HandType type = hand.getType();
	switch (type)
	{
	case PlayHand::Hand_Unknown:
		break;
	case PlayHand::Hand_Pass:
		break;
	case PlayHand::Hand_Single:
		break;
	case PlayHand::Hand_Pair:
		break;
	case PlayHand::Hand_Triple:
		break;
	case PlayHand::Hand_Triple_Single:
		break;
	case PlayHand::Hand_Triple_Pair:
		break;
	case PlayHand::Hand_Plane:
		showAnimation(PLANE);
		break;
	case PlayHand::Hand_Plane_Two_Single:
		showAnimation(PLANE);
		break;
	case PlayHand::Hand_Plane_Two_Pair:
		showAnimation(PLANE);
		break;
	case PlayHand::Hand_Seq_Pair:
		showAnimation(LIANDUI);
		break;
	case PlayHand::Hand_Seq_Single:
		showAnimation(SHUNZI);
		break;
	case PlayHand::Hand_Bomb:
		showAnimation(BOMB);
		break;
	case PlayHand::Hand_Bomb_Single:
		break;
	case PlayHand::Hand_Bomb_Pair:
		break;
	case PlayHand::Hand_Bomb_Two_Single:
		break;
	case PlayHand::Hand_Bomb_Jokers:
		showAnimation(BOMB);
		break;
	case PlayHand::Hand_Bomb_Jokers_Single:
		break;
	case PlayHand::Hand_Bomb_Jokers_Pair:
		break;
	case PlayHand::Hand_Bomb_Jokers_Two_Single:
		break;
	default:
		break;
	}
	//如果玩家打出的是空牌，显示提示信息
	if (cards.isEmpty()) {
		it->info->setPixmap(QPixmap(":/LordGame/image/pass.png"));
		it->info->show();
	}
	//3.更新玩家剩余的牌
	updatePlayerCards(player);
	//4.播放提示音效

}

void gamePanel::showAnimation(AnimationType type, int bet)
{
	switch (type)
	{
	case gamePanel::SHUNZI:
		break;
	case gamePanel::LIANDUI:
		break;
	case gamePanel::PLANE:
		break;
	case gamePanel::JOKERBOMB:
		break;
	case gamePanel::BOMB:
		break;
	case gamePanel::BET:
		m_animation->setFixedSize(160, 98);
		m_animation->move((width() - m_animation->width()) / 2, (height() - m_animation->height()) / 2 - 140);
		m_animation->showBetScore(bet);
		break;
	default:
		break;
	}
	m_animation->show();
}

void gamePanel::hidePlayerDropCards(Player* player)
{
	auto it = m_contextMap.find(player);
	if (it != m_contextMap.end()) {
		if (it->lastCards.isEmpty()) {
			it->info->hide();
		}
		else {//隐藏
			//Cards-->Card
			CardLsit list = it->lastCards.toCardList();
			for (auto last = list.begin(); last != list.end(); ++last) {
				m_cardMap[*last]->hide();//隐藏cardPanel对象
			}
		}
	}
}

void gamePanel::onPlayerStatusChanged(Player* player, GameControl::PlayerStatus status)
{
	switch (status)
	{
	case GameControl::THINKCALLLORD:
		//玩家使用
		if(player==m_gameCtl->getUserPlayer())
			ui.buttonGroup->selectPanel(ButtonGroup::CALLLORD, m_gameCtl->getPlayerMaxBet());
		break;
	case GameControl::THINKPLAYHAND:
		break;
	case GameControl::WINNING:
		break;
	default:
		break;
	}
}

void gamePanel::onGrabLordBet(Player* player, int bet, bool flag)
{
	//显示抢地主的信息提示
	PlayerContext context = m_contextMap[player];
	if (bet == 0) {
		QPixmap pixmap;
		pixmap.load(":/LordGame/image/buqinag.png");
		context.info->setPixmap(pixmap);
	}
	else {
		if (flag) {
			QPixmap pixmap;
			pixmap.load(":/LordGame/image/jiaodizhu.png");
			context.info->setPixmap(pixmap);
		}
		else {
			QPixmap pixmap;
			pixmap.load(":/LordGame/image/qiangdizhu.png");
			context.info->setPixmap(pixmap);
		}
	}
	context.info->show();

	//显示叫地主的分数
	showAnimation(gamePanel::BET, bet);
	//播放分数的音乐
}

void gamePanel::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), m_backGroundPixmap);
}
