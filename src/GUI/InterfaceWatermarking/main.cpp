#include "mainwindow.h"
#include <QApplication>

#include "libwrapper.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	LibWrapper* libWrapper = new LibWrapper(w.getUi());

	return a.exec();
}
