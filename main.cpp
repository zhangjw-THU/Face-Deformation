#include "facedistorted.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Facedistorted w;
	QString info=info.fromLocal8Bit("»À¡≥±‰–Œ");
	w.setWindowTitle(info);
	w.show();
	return a.exec();
}
