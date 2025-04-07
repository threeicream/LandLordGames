#include <QApplication>
#include <QWidget>
#include "gamePanel.h"
#include <QPainter>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	gamePanel w;
	w.show();
	return /*QApplication::*/a.exec();
}