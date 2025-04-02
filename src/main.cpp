#include <QApplication>
#include <QWidget>
#include "MainWindow.h"
#include <QPainter>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	MainWindow w;
	w.show();
	return /*QApplication::*/a.exec();
}