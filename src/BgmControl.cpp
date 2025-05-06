#include "BgmControl.h"
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QAudioOutput>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include "PlayHand.h"
#include <QTimer>

BgmControl::BgmControl(QObject *parent)
	: QObject(parent)
{
	for (int i = 0; i < 5; ++i) {
		QMediaPlayer* player = new QMediaPlayer(this);
		QSoundEffect* effect = new QSoundEffect(this);
		QAudioOutput* audio = new QAudioOutput(this);
		if (i < 2 || i == 4) {
			player->setLoops(0);
		}
		else if (i == 2) {
			player->setLoops(QMediaPlayer::Infinite);
		}
		audio->setVolume(1);
		player->setAudioOutput(audio);
		
		effect->setVolume(1);
		//effect->setAudioDevice(m_audio);
		m_player.append(player);
		m_effect.append(effect);
		m_audio.append(audio);
	}
	initPlayList();
}

BgmControl::~BgmControl()
{
}

void BgmControl::initPlayList()
{
	QStringList list;
	list << "Man" << "Woman" << "BGM" << "Ending" << "Other";
	m_list.resize(5);
	//读取json文件
	QFile file(":/LordGame/conf/music.json");
	QByteArray json;
	if (file.open(QFile::ReadOnly)) {
		json = file.readAll();
	}
	else
		return;
	file.close();
	//解析json数据
	QJsonDocument doc = QJsonDocument::fromJson(json);
	if (doc.isNull()) {
		qDebug() << "Failed to parse JSON";
		return; // 注意要返回，防止后续操作
	}

	QJsonObject obj = doc.object();

	for (int i = 0; i < list.size(); ++i) {
		QString prefix = list[i];
		QJsonArray array = obj.value(prefix).toArray();
		m_list[i].clear();
		for (int j = 0; j < array.size(); ++j) {
			QUrl url = QUrl::fromUserInput(array[j].toString());
			if (!url.isValid()) {
				qDebug() << "Invalid URL: " << url.toString();
				continue;
			}
			m_list[i].append(url);
		}
	}
	//初始化多媒体播放列表
}

void BgmControl::startBGM(float volume)
{
	int num = QRandomGenerator::global()->bounded(7);
	m_audio[2]->setVolume(volume);
	m_player[2]->setSource(m_list[2][num]);
	m_player[2]->play();
}

void BgmControl::stopBGM()
{
	m_player[2]->stop();
}

//玩家是否下注？
//玩家性别
//什么时候播放什么样的音频文件
void BgmControl::PlayerRobLordMusic(int point, Sex sex, bool isFirst)
{
	int index = sex == Sex::MAN ? 0 : 1;
	if (isFirst && point) {
		m_player[index]->setSource(m_list[index][CardType::Order]);
	}
	else if (point == 0) {
		if (isFirst) {
			m_player[index]->setSource(m_list[index][CardType::NoOrder]);
		}
		else {
			m_player[index]->setSource(m_list[index][CardType::NoRob]);
		}
	}
	else if (point == 2) {
		m_player[index]->setSource(m_list[index][CardType::Rob1]);
	}
	else if (point == 3) {
		m_player[index]->setSource(m_list[index][CardType::Rob2]);
	}
	m_player[index]->play();
}

void BgmControl::PlayCardMusic(Cards cards, bool isFirst, Sex sex)
{
	//得到播放列表
	int index = sex == Sex::MAN ? 0 : 1;
	Card::CardPoint pt = Card::Card_Begin;
	//取出牌型 然后进行判断
	PlayHand hand(cards);
	PlayHand::HandType type = hand.getType();
	if (type == PlayHand::Hand_Single || type == PlayHand::Hand_Pair || type == PlayHand::Hand_Triple) {
		pt = cards.takeRandCard().getPoint();
	}
	switch (type)
	{
	case PlayHand::Hand_Single:
		m_player[index]->setSource(m_list[index][CardType::Three + pt - 1]);
		break;
	case PlayHand::Hand_Pair:
		m_player[index]->setSource(m_list[index][CardType::Three + pt - 1 + 15]);
		break;
	case PlayHand::Hand_Triple:
		m_player[index]->setSource(m_list[index][CardType::Three + pt - 1 + 28]);
		break;
	case PlayHand::Hand_Triple_Single:
		m_player[index]->setSource(m_list[index][CardType::ThreeBindOne]);
		break;
	case PlayHand::Hand_Triple_Pair:
		m_player[index]->setSource(m_list[index][CardType::ThreeBindPair]);
		break;
	case PlayHand::Hand_Plane:
		m_player[index]->setSource(m_list[index][CardType::Plane]);
		break;
	case PlayHand::Hand_Plane_Two_Single:
		m_player[index]->setSource(m_list[index][CardType::Plane]);
		break;
	case PlayHand::Hand_Plane_Two_Pair:
		m_player[index]->setSource(m_list[index][CardType::Plane]);
		break;
	case PlayHand::Hand_Seq_Pair:
		m_player[index]->setSource(m_list[index][CardType::SequencePair]);
		break;
	case PlayHand::Hand_Seq_Single:
		m_player[index]->setSource(m_list[index][CardType::Sequence]);
		break;
	case PlayHand::Hand_Bomb:
		m_player[index]->setSource(m_list[index][CardType::Bomb]);
		break;
	case PlayHand::Hand_Bomb_Two_Pair:
		m_player[index]->setSource(m_list[index][CardType::FourBind2Pair]);
		break;
	case PlayHand::Hand_Bomb_Pair:
		m_player[index]->setSource(m_list[index][CardType::FourBindTwo]);
		break;
	case PlayHand::Hand_Bomb_Two_Single:
		m_player[index]->setSource(m_list[index][CardType::FourBindTwo]);
		break;
	case PlayHand::Hand_Bomb_Jokers:
		m_player[index]->setSource(m_list[index][CardType::JokerBomb]);
		break;
	case PlayHand::Hand_Bomb_Jokers_Two_Pair:
		m_player[index]->setSource(m_list[index][CardType::FourBind2Pair]);
		break;
	case PlayHand::Hand_Bomb_Jokers_Pair:
		m_player[index]->setSource(m_list[index][CardType::FourBindTwo]);
		break;
	case PlayHand::Hand_Bomb_Jokers_Two_Single:
		m_player[index]->setSource(m_list[index][CardType::FourBindTwo]);
		break;
	default:
		break;
	}
	if (!isFirst && type >= PlayHand::Hand_Triple && type < PlayHand::Hand_Bomb) {
		int num = QRandomGenerator::global()->bounded(3);
		m_player[index]->setSource(m_list[index][CardType::MoreBiger1 + num]);
	}
	//播放音乐
	m_player[index]->play();
}

void BgmControl::PlayPassMusic(Sex sex)
{
	int index = sex == Sex::MAN ? 0 : 1;
	int num = QRandomGenerator::global()->bounded(4);
	m_player[index]->setSource(m_list[index][CardType::Pass1 + num]);
	m_player[index]->play();
}

void BgmControl::PlaySpecialMusic(AssistMusic type)
{
	if (type == Dispatch) {
		//循环
		m_player[4]->setLoops(QMediaPlayer::Infinite);
	}
	else {
		//单曲播放
		m_player[4]->setLoops(1);
	}
	m_player[4]->setSource(m_list[4][type]);
	m_player[4]->play();
}

void BgmControl::stopSpecialMusic()
{
	m_player[4]->stop();
}

void BgmControl::PlayEndingMusic(bool iswin)
{
	if (iswin) {
		m_player[3]->setSource(m_list[3][2]);
	}
	else {
		m_player[3]->setSource(m_list[3][3]);
	}
	m_audio[3]->setVolume(0.5);
	m_player[3]->play();
}

void BgmControl::stopEndingMusic()
{
	m_player[3]->stop();
}

void BgmControl::PlayLastMusic(BgmControl::CardType type, Sex sex)
{
	int index = sex == Sex::MAN ? 0 : 1;
	m_player[index]->setSource(m_list[index][type]);
	m_player[index]->play();
	/*if (m_player[index]->playbackState() == QMediaPlayer::StoppedState) {
		m_player[index]->setSource(m_list[index][type]);
		m_player[index]->play();
	}
	else {
		QTimer::singleShot(1000, this, [&]() {
			m_player[index]->setSource(m_list[index][type]);
			m_player[index]->play();
			});
	}*/
	
}
