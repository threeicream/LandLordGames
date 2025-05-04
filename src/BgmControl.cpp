#include "BgmControl.h"
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QAudioOutput>
//#include <QMediaSource>

BgmControl::BgmControl(QObject *parent)
	: QObject(parent)
{
	m_audio = new QAudioOutput(this);
	m_audio->setVolume(100);
	for (int i = 0; i < 5; ++i) {
		QMediaPlayer* player = new QMediaPlayer(this);
		QSoundEffect* effect = new QSoundEffect(this);
		player->setAudioOutput(m_audio);
		effect->setVolume(1);
		//effect->setAudioDevice(m_audio);
		m_player.append(player);
		m_effect.append(effect);
	}
}

BgmControl::~BgmControl()
{
}
