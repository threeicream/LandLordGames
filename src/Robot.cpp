#include "Robot.h"
#include "Strategy.h"
#include "RobotGrapLord.h"
#include "RobotPlayHand.h"
#include <QThread>
#include "Strategy.h"

void Robot::prepareCallLord()
{
	////创建线程类
	//RobotGrapLord* subThread = new RobotGrapLord(this);
	//subThread->start();

	//connect(subThread, &RobotGrapLord::finished, subThread, &RobotGrapLord::deleteLater);
	//connect(subThread, &RobotGrapLord::finished, this, [=]() {//使用=和lambda表达式不是很安全
	//	if (subThread) { // 检查 Worker1 是否为 nullptr
	//		if (subThread->isRunning()) {
	//			subThread->quit();
	//			subThread->wait();
	//		}
	//	}
	//	});

	QThread* Worker1 = new QThread();
	robotGrapLord* sub = new robotGrapLord(this, nullptr);
	sub->moveToThread(Worker1);
	connect(Worker1, &QThread::started, sub, &robotGrapLord::workGrapLord);
	connect(Worker1, &QThread::finished, this, [=]() {
		if (Worker1) { // 检查 Worker1 是否为 nullptr
			if (Worker1->isRunning()) {
				Worker1->quit();
				Worker1->wait();
			}
		}
		}); // 确保线程结束时释放内存
	connect(Worker1, &QThread::finished, Worker1, &QThread::deleteLater); // 确保线程结束时释放内存

	Worker1->start();
}

void Robot::preparePlayHand()
{
	////创建线程类
	//RobotPlayHand* subThread = new RobotPlayHand(this);
	//subThread->start();

	//connect(subThread, &RobotPlayHand::finished, subThread, &RobotPlayHand::deleteLater);
	//connect(subThread, &RobotPlayHand::finished, this, [=]() {//使用=和lambda表达式不是很安全
	//	if (subThread) { // 检查 Worker1 是否为 nullptr
	//		if (subThread->isRunning()) {
	//			subThread->quit();
	//			subThread->wait();
	//		}
	//	}
	//	});
	QThread* Worker2 = new QThread();
	robotGrapLord* sub = new robotGrapLord(this, nullptr);
	sub->moveToThread(Worker2);
	connect(Worker2, &QThread::started, sub, &robotGrapLord::workPlayHand);
	connect(Worker2, &QThread::finished, this, [=]() {
		if (Worker2) { // 检查 Worker1 是否为 nullptr
			if (Worker2->isRunning()) {
				Worker2->quit();
				Worker2->wait();
			}
		}
		}); // 确保线程结束时释放内存
	connect(Worker2, &QThread::finished, Worker2, &QThread::deleteLater); // 确保线程结束时释放内存

	Worker2->start();
}

void Robot::thinkCallLorad()
{
	/*
	基于手牌计算权重
	1.大小王：6
	2.顺子/炸弹：5
	3.三张点数相同的牌：4
	4.2：3
	5.对牌：1
	*/
	int weight = 0;
	Strategy st(this, m_cards);
	weight += st.findRangeCards(Card::Card_SJ, Card::Card_BJ).cardCount() * 6;

	QVector<Cards> optSeq = st.pickOptimalSeqSingle();
	for (auto& it : optSeq) {//测试
		it.getAllCardsPoint();
	}
	weight += optSeq.size() * 5;

	QVector<Cards> bombs = st.findCardsByCount(4);
	weight += bombs.size() * 5;

	weight += m_cards.pointCount(Card::Card_2) * 3;
	Cards tmp = m_cards;
	tmp.delCard(optSeq);
	tmp.delCard(bombs);
	Cards card2 = st.findRangeCards(Card::Card_2,Card::Card_2);
	tmp.delCard(card2);

	QVector<Cards> triples = Strategy(this, tmp).findCardsByCount(3);
	weight += triples.size() * 4;

	tmp.delCard(triples);
	QVector<Cards> pairs = Strategy(this, tmp).findCardsByCount(2);
	weight += pairs.size() * 1;

	//计算分数
	if (weight >= 22) {
		grabLordBet(3);
	}
	else if (weight >= 18) {
		grabLordBet(2);
	}
	else if (weight >= 10) {
		grabLordBet(1);
	}
	else
		grabLordBet(0);
}

void Robot::thinkPlayHand()
{
	Strategy st(this, m_cards);
	Cards cs = st.makeStrategy();
	playHand(cs);
}

void Robot::thinkPlayHand_api()
{
	//str->makeStrategyApi();
}

Robot::Robot(QObject *parent)
	: Player(parent)
{
	this->m_tyep = Player::ROBOT;
	//str = new Strategy(this, m_cards);
	//connect(str, &Strategy::cardsToPlayReady, this, &Robot::playHand);
}

Robot::~Robot()
{
	
}
