#include "mainwindow.h"
#include <QApplication>

#include "libwrapper.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	LibWrapper libWrapper(w.getUi());

	w.show();

	return a.exec();
}
