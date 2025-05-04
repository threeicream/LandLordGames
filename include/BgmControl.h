#pragma once

#include <QObject>
class QMediaPlayer;
class QSoundEffect;
class QAudioOutput;

class BgmControl : public QObject
{
	Q_OBJECT

public:
	BgmControl(QObject* parent = nullptr);
	~BgmControl();
private:
	//0man 1woman 2bgm 3辅助音乐 4结束音乐 
	QVector<QMediaPlayer*> m_player;
	QVector<QSoundEffect*> m_effect;
	QAudioOutput* m_audio = nullptr;
};
