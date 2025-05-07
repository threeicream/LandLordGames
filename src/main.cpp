#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QFile>
#include "Loading.h"
#include "Cards.h"
#include <QResource>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	qRegisterMetaType<Cards>("Cards&"); // 注册 Cards& 类型
	/*QFile stylefile(":/LordGame/stylesheet/style1.qss");
	if (stylefile.open(QFile::ReadOnly)) {
		QString stylesheet = QString(stylefile.readAll());
		a.setStyleSheet(stylesheet);
		stylefile.close();
	}*/
	QResource::registerResource("./LordGame.rcc");
	Loading y;
	y.show();
	y.startLoading();
	return /*QApplication::*/a.exec();
	
}