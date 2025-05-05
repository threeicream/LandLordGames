#pragma once

#include <QObject>
#include <QUrl>
#include "Cards.h"
class QMediaPlayer;
class QSoundEffect;
class QAudioOutput;

class BgmControl : public QObject
{
	Q_OBJECT
public:
	enum Sex { MAN, WOMAN };
	enum CardType
	{
		// 单张牌
		Three,
		Foue,
		Five,
		Six,
		Seven,
		Eight,
		Nine,
		Ten,
		Jack,
		Queen,
		King,
		Ace,
		Tow,
		SmallJoker,
		BigJoker,
		// 两张牌
		Three_Double,
		Foue_Double,
		Five_Double,
		Six_Double,
		Seven_Double,
		Eight_Double,
		Nine_Double,
		Ten_Double,
		Jack_Double,
		Queen_Double,
		King_Double,
		Ace_Double,
		Tow_Double,
		// 三张牌
		Three_Triple,
		Foue_Triple,
		Five_Triple,
		Six_Triple,
		Seven_Triple,
		Eight_Triple,
		Nine_Triple,
		Ten_Triple,
		Jack_Triple,
		Queen_Triple,
		King_Triple,
		Ace_Triple,
		Tow_Triple,
		// 其他组合
		Plane,          // 飞机
		SequencePair,   // 连对
		ThreeBindOne,   // 三带一
		ThreeBindPair,  // 三带一对
		Sequence,       // 顺子
		FourBindTwo,    // 四带二(单张)
		FourBind2Pair,  // 四带两对
		Bomb,           // 炸弹
		JokerBomb,      // 王炸
		Pass1,          // 过
		Pass2,
		Pass3,
		Pass4,
		MoreBiger1,     // 大你
		MoreBiger2,
		Biggest,        // 压死
		// 抢地主
		NoOrder,        // 不叫
		NoRob,          // 不抢
		Order,          // 叫地主
		Rob1,           // 抢地主
		Rob2,
		Last1,          // 只剩1张牌
		Last2           // 只剩2张牌
	};

	enum AssistMusic
	{
		Dispatch,   // 发牌
		SelectCard, // 选牌
		PlaneVoice, // 飞机
		BombVoice,  // 炸弹
		Alert,      // 提醒
		Shunzi=18   // 顺子
	};
public:
	BgmControl(QObject* parent = nullptr);
	~BgmControl();

	//初始化播放列表
	void initPlayList();

	//1.背景音乐（播放和停止）
	void startBGM(float volume);
	void stopBGM();
	//2.播放抢地主音乐
	void PlayerRobLordMusic(int point, Sex sex, bool isFirst);
	//3.播放出牌/不出牌音乐
	void PlayCardMusic(Cards cards, bool isFirst, Sex sex);
	void PlayPassMusic(Sex sex);
	//4.特殊牌型音乐
	void PlaySpecialMusic(AssistMusic type);
	void stopSpecialMusic();
	//5.结算音乐
	void PlayEndingMusic(bool iswin);
	void stopEndingMusic();
	//6.最后两张牌
	void PlayLastMusic(BgmControl::CardType type, Sex sex);
private:
	//0man 1woman 2bgm 3辅助音乐 4结束音乐 
	QVector<QMediaPlayer*> m_player;
	QVector<QSoundEffect*> m_effect;
	QVector<QAudioOutput*> m_audio;
	QVector<QVector<QUrl>> m_list;
};
