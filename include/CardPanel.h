#pragma once

#include <QWidget>
#include <QPixmap>
#include "Card.h"
#include "Player.h"

class QPaintEvent;

class CardPanel : public QWidget
{
	Q_OBJECT

public:
	CardPanel(QWidget* parent = nullptr);
	~CardPanel();
	//设置获取图片函数
	void setImage(QPixmap& front, QPixmap& back);
	QPixmap getFrontImage() const;

	//扑克牌显式哪一面
	void setFrontSide(bool flag);
	bool isFrontSide();

	//记录窗口是否被选中
	void setSelected(bool flag);
	bool isSelected();

	//扑克牌的花色以及点数
	void setCard(Card& card);//引用为关联关系，两个类之间存在某种联系，但它们可以独立存在
	Card getCard();

	//扑克牌的所有者
	void setOwner(Player* player);//指针为组合关系，一个类 拥有 另一个类，并且被拥有的类不能独立存在
	Player* getOwner();

	//模拟扑克牌点击事件
	void clicked();
signals:
	void cardSelected(Qt::MouseButton button);
protected:
	virtual void paintEvent(QPaintEvent* event)override;
	virtual void mousePressEvent(QMouseEvent* event)override;
private:
	QPixmap m_front;
	QPixmap m_back;
	bool m_isFront;
	bool m_isSelected;
	Card m_card;
	Player* m_owner;
};
