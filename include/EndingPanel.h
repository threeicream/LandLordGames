#pragma once

#include <QWidget>
class QPixmap;
class QLabel;
class ScorePanel;
class QPushButton;

class EndingPanel : public QWidget
{
	Q_OBJECT

public:
	EndingPanel(bool isLord, bool isWin, QWidget* parent = nullptr);
	~EndingPanel();

	//设置分数
	void setMyScore(int left,int right,int user);

	//上下移动
	void zoomY(int y);
signals:
	void continueGame();
protected:
	virtual void paintEvent(QPaintEvent* event)override;
private:
	QPixmap m_picture;
	QLabel* m_title = nullptr;
	ScorePanel* m_score = nullptr;
	QPushButton* m_continue = nullptr;
};
