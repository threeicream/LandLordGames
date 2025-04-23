#include "Player.h"

Player::Player(QObject *parent)
	: QObject(parent)
{
	m_score = 0;
	m_isWin = false;
}

Player::Player(QString name, QObject* parent /*= nullptr*/)
	:Player(parent)//委托构造函数
{
	m_name = name;
}

Player::~Player()
{
}

void Player::setName(QString name)
{
	m_name = name;
}

QString Player::getName()const
{
	return m_name;
}

void Player::setRole(Player::Role role)
{
	m_role = role;
}

Player::Role Player::getRole() const
{
	return m_role;
}

void Player::setSex(Player::Sex sex)
{
	m_sex = sex;
}

Player::Sex Player::getSex() const
{
	return m_sex;
}

void Player::setDirection(Player::Direction direction)
{
	m_direction = direction;
}

Player::Direction Player::getDirection() const
{
	return m_direction;
}

void Player::setType(Player::Type type)
{
	m_tyep = type;
}

Player::Type Player::getType() const
{
	return m_tyep;
}

void Player::setScore(int score)
{
	m_score = score;
}

int Player::getScore()const
{
	return m_score;
}

void Player::setIsWin(bool isWin)
{
	m_isWin = isWin;
}

bool Player::getIsWin() const
{
	return m_isWin;
}

void Player::setPrevPlayer(Player* player)
{
	m_PrevPlayer = player;
}

void Player::setNextPlayer(Player* player)
{
	m_NextPlayer = player;
}

Player* Player::getPrevPlayer() const
{
	return m_PrevPlayer;
}

Player* Player::getNextPlayer() const
{
	return m_NextPlayer;
}

void Player::grabLordBet(int point)
{
	emit notifyGrabLordBet(this, point);
}

void Player::storeDispatchCard(Card& card)
{
	m_cards.addCard(card);
	Cards cs;
	cs.addCard(card);
}

void Player::storeDispatchCards(Cards& cards)
{
	m_cards.addCard(cards);
}

Cards Player::getCards()const
{
	return m_cards;
}

void Player::clearCards()
{
	m_cards.clear();
}

void Player::playHand(Cards& cards)
{
	m_cards.delCard(cards);
	emit notifyPlayHand(this, cards);
}

void Player::setPendingInfo(Player* player, const Cards& cards)
{
	m_PrevPlayer = player;
	m_pendCards = cards; 
}

Player* Player::getPendPlayer() const
{
	return m_pendPlayer;
}

Cards Player::getPendCards() const
{
	return m_pendCards;
}

void Player::storePendingInfo(Player* player, Cards& cards)
{
	m_PrevPlayer = player;
	m_pendCards = cards;
}

void Player::prepareCallLord()//准备抢地主
{
}

void Player::preparePlayHand()
{
}

