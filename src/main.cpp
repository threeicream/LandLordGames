#include <QApplication>
#include <QWidget>
#include "gamePanel.h"
#include <QPainter>
#include <QFile>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	/*QFile stylefile(":/LordGame/stylesheet/style1.qss");
	if (stylefile.open(QFile::ReadOnly)) {
		QString stylesheet = QString(stylefile.readAll());
		a.setStyleSheet(stylesheet);
		stylefile.close();
	}*/

	gamePanel w;
	w.show();
	return /*QApplication::*/a.exec();
}