#include <QtWidgets/QApplication>

#include "Aircraft.h"
#include "Enemy.h"
#include "cxGameView.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	srand(QDateTime::currentSecsSinceEpoch());

 	cxGameView view;
 	view.show();
	return a.exec();
}
