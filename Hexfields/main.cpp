#include "stdafx.h"
#include <HexRedactor.h>
#include <QtWidgets/QApplication>
#include <Demo.h>
int main(int argc, char *argv[])
{
	using namespace HexCoords;
	QApplication a(argc, argv);
	Demo * demo = new Demo();
	//demo->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	demo->showMaximized();
	//DijkstraDemo * ddemo = new DijkstraDemo(
	//	std::string("weightsmap.txt"), 50, 500, lineopts,QSize(500,500));
	//ddemo->showMaximized();
	//HexRedactor * red = new HexRedactor(std::string("weightsmap.txt"), QSize(500, 500), HEX_DEFAULTS);
	//red->showMaximized();
	return a.exec();
}
