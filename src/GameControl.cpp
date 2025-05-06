#include "GameControl.h"
#include <QRandomGenerator>
#include <QTimer>
#include "PlayHand.h"

void GameControl::playerInit()
{
	//对象实例化
	m_robotLeft = new Robot("机器人A", this);
	m_robotRight = new Robot("机器人B", this);
	m_user = new UserPlayer("玩家", this);

	//头像显示方位
	m_robotLeft->setDirection(Player::LEFT);
	m_robotRight->setDirection(Player::RIGHT);
	m_user->setDirection(Player::RIGHT);

	//性别
	m_robotLeft->setSex(static_cast<Player::Sex>(QRandomGenerator::global()->bounded(2)));
	m_robotRight->setSex(static_cast<Player::Sex>(QRandomGenerator::global()->bounded(2)));
	m_user->setSex(static_cast<Player::Sex>(QRandomGenerator::global()->bounded(2)));

	//出牌顺序 逆时针出牌
	m_user->setPrevPlayer(m_robotLeft);
	m_user->setNextPlayer(m_robotRight);
	m_robotLeft->setPrevPlayer(m_robotRight);
	m_robotLeft->setNextPlayer(m_user);
	m_robotRight->setPrevPlayer(m_user);
	m_robotRight->setNextPlayer(m_robotLeft);

	//指定当前玩家
	m_currPlayer = m_user;

	//处理玩家发射出的信号
	connect(m_user, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
	connect(m_robotLeft, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
	connect(m_robotRight, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);

	//传递出牌玩家和打出的牌
	connect(this, &GameControl::pendingInfo, m_robotLeft, &Robot::storePendingInfo);
	connect(this, &GameControl::pendingInfo, m_robotRight, &Robot::storePendingInfo);
	connect(this, &GameControl::pendingInfo, m_user, &UserPlayer::storePendingInfo);

	//处理玩家出牌
	connect(m_robotLeft, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
	connect(m_robotRight, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
	connect(m_user, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);

}

Robot* GameControl::getLeftRobot()const
{
	return m_robotLeft;
}

Robot* GameControl::getRightRobot()const
{
	return m_robotRight;
}

UserPlayer* GameControl::getUserPlayer()const
{
	return m_user;
}

void GameControl::setCurrentPlayer(Player* player)
{
	m_currPlayer = player;
}

Player* GameControl::getCurrentPlayer()const
{
	return m_currPlayer;
}

Player* GameControl::getPendPlayer() const
{
	return m_pendPlayer;
}

Cards GameControl::getPendCards() const
{
	return m_pendCards;
}

void GameControl::allCardsInit()
{
	m_allCards.clear();
	for (auto i = Card::Suit_Begin + 1; i < Card::Suit_End; ++i) {
		for (auto j = Card::Card_Begin + 1; j < Card::Card_SJ; ++j) {
			Card card(static_cast<Card::CardPoint>(j), static_cast<Card::CardSuit>(i));
			m_allCards.addCard(card);
		}
	}
	m_allCards.addCard(Card(Card::Card_SJ, Card::Suit_Begin));
	m_allCards.addCard(Card(Card::Card_BJ, Card::Suit_Begin));

}

Card GameControl::takeOneCard()
{
	return m_allCards.takeRandCard();
}

Cards GameControl::getSurplusCards() const
{
	return m_allCards;
}

void GameControl::resetCardData()
{
	//洗牌
	allCardsInit();
	//清空玩家手牌
	m_robotLeft->clearCards();
	m_robotRight->clearCards();
	m_user->clearCards();
	//初始化出牌玩家对象和牌
	m_pendPlayer = nullptr;
	m_pendCards.clear();
}

void GameControl::startLordCard()
{
	m_currPlayer->prepareCallLord();
	emit playerStatusChanged(m_currPlayer, PlayerStatus::THINKCALLLORD);
}

void GameControl::becomeLord(Player* player, int bet)
{
	player->setRole(Player::LANDLORD);
	player->getPrevPlayer()->setRole(Player::FARMER);
	player->getNextPlayer()->setRole(Player::FARMER);

	m_currPlayer = player;
	m_curBet = bet;
	player->storeDispatchCards(m_allCards);//三张底牌给地主
	QTimer::singleShot(1000, this, [=]() {
		//游戏状态改变
		emit gameStatusChanged(GameStatus::PLAYINGHAND);
		//玩家状态改变
		emit playerStatusChanged(m_currPlayer,PlayerStatus::THINKPLAYHAND);
		m_currPlayer->preparePlayHand();//地主准备出牌
		});
}

void GameControl::clearPlayerScore()
{
	m_robotLeft->setScore(0);
	m_robotRight->setScore(0);
	m_user->setScore(0);
}

int GameControl::getPlayerMaxBet()
{
	return m_betRecord.bet;
}

void GameControl::testData()
{
	//清空玩家手牌
	m_robotLeft->clearCards();
	m_robotRight->clearCards();
	m_user->clearCards();
	//初始化出牌玩家对象和牌
	m_pendPlayer = nullptr;
	m_pendCards.clear();
}

void GameControl::onPlayHand(Player* player, Cards& cards)
{
	//1.将玩家出牌的信号转发主界面
	emit notifyPlayHand(player, cards);
	//2.如果不是空牌，给其他玩家发送信号，使其保存出牌玩家和打出的牌
	if (!cards.isEmpty()) {
		m_pendCards = cards;
		m_pendPlayer = player;
		player->setPass(false);
		emit pendingInfo(player, cards);
	}
	else {
		player->setPass(true);
	}

	//如果有炸弹，翻倍底分
	PlayHand::HandType type = PlayHand(cards).getType();
	if (type == PlayHand::Hand_Bomb || type == PlayHand::Hand_Bomb_Jokers) {
		m_curBet *= 2;
	}

	//3.如果玩家的牌出完了，计算本局得分
	Player* prev = player->getPrevPlayer();
	Player* next = player->getNextPlayer();
	if (player->getCards().isEmpty()) {
		if (player->getRole() == Player::LANDLORD) {
			player->setScore(player->getScore() + 2 * m_curBet);
			prev->setScore(prev->getScore() - m_curBet);
			next->setScore(next->getScore() - m_curBet);
			
			player->setIsWin(true);
			prev->setIsWin(false);
			next->setIsWin(false);
		}
		else if (player->getRole() == Player::FARMER && prev->getRole() == Player::FARMER) {
			player->setScore(player->getScore() + m_curBet);
			prev->setScore(prev->getScore() + m_curBet);
			next->setScore(next->getScore() - 2 * m_curBet);

			player->setIsWin(true);
			prev->setIsWin(true);
			next->setIsWin(false);
		}
		else if (player->getRole() == Player::FARMER && next->getRole() == Player::FARMER) {
			player->setScore(player->getScore() + m_curBet);
			next->setScore(next->getScore() + m_curBet);
			prev->setScore(prev->getScore() - 2 * m_curBet);

			player->setIsWin(true);
			prev->setIsWin(false);
			next->setIsWin(true);
		}
		emit playerStatusChanged(player, GameControl::WINNING);
		return;
	}
	//4.如果牌没有出完，下一个玩家继续出牌
	m_currPlayer = player->getNextPlayer();
	m_currPlayer->preparePlayHand();
	emit playerStatusChanged(m_currPlayer, GameControl::THINKPLAYHAND);
}

void GameControl::onGrabBet(Player* player, int bet)
{
	//1.通知主窗口 玩家叫地主了（更新信息提示）
	if (bet == 0 || m_betRecord.bet >= bet) {//如果小于最大分数，那么就是不要地主
		emit notifyGrabLordBet(player, 0, false);
	}
	else if (bet > 0 && m_betRecord.bet == 0) {//第一个抢地主的玩家
		emit notifyGrabLordBet(player, bet, true);
	}
	else {//其他两个
		emit notifyGrabLordBet(player, bet, false);
	}
	//2.判断玩家下注是否为3分，是则抢地主结束
	if (bet == 3) {
		//玩家成为地主
		becomeLord(player, bet);
		//清空record数据
		m_betRecord.reset();
		return;
	}
	//3.如果不是3分，切换下一个玩家继续抢地主
	if (m_betRecord.bet < bet) {
		m_betRecord.bet = bet;
		m_betRecord.player = player;
	}
	++m_betRecord.times;
	//如果都抢过地主，抢地主结束
	if (m_betRecord.times == 3) {
		if (m_betRecord.bet == 0) {
			emit gameStatusChanged(DISPATCHCARD);
		}
		else {
			becomeLord(m_betRecord.player, m_betRecord.bet);
		}
		m_betRecord.reset();
		return;
	}
	//4.切换玩家，通知下一个玩家继续抢地主
	m_currPlayer = player->getNextPlayer();
	//发送信号给主界面，告诉当前状态为抢地主
	emit playerStatusChanged(m_currPlayer, PlayerStatus::THINKCALLLORD);
	m_currPlayer->prepareCallLord();
}

GameControl::GameControl(QObject *parent)
	: QObject(parent)
{
	playerInit();
	allCardsInit();

	//不能放在这里，会无法触发信号函数
	////处理玩家发射出的信号
	//connect(m_user, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
	//connect(m_robotLeft, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
	//connect(m_robotRight, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);

}

GameControl::~GameControl()
{
}
