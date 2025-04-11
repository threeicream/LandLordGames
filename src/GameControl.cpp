#include "GameControl.h"
#include <QRandomGenerator>

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

void GameControl::becomeLord(Player* player)
{
	player->setRole(Player::LANDLORD);
	player->getPrevPlayer()->setRole(Player::FARMER);
	player->getNextPlayer()->setRole(Player::FARMER);

	m_currPlayer = player;
	player->storeDispatchCards(m_allCards);

	m_currPlayer->preparePlayHand();
}

void GameControl::clearPlayerScore()
{
	m_robotLeft->setScore(0);
	m_robotRight->setScore(0);
	m_user->setScore(0);
}

GameControl::GameControl(QObject *parent)
	: QObject(parent)
{
	playerInit();
	allCardsInit();
}

GameControl::~GameControl()
{
}
