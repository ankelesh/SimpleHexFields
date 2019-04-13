#include "stdafx.h"
#include "HexfieldWidget.h"
#include "CubeCoords.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Hexfields w;
	w.show();
	ICube fcube(3, 1, -4);
	ICube scube(1, 0, -1);
	debug << "First test: summ " << (fcube + scube).toStr()
		<< "\nSecond test: minus " << (fcube - scube).toStr()
		<< "\nThird test: distance " << fcube.distanceTo(scube)
		<< "\nForth test: create line -> begins at " << fcube.toStr();
	for each(auto a in fcube.createLine(scube))
	{
		debug << "\n" << a.toStr();
	}
	debug << "\nends at-> " << scube.toStr();
	debug << "\nTest 5: make range from -> " << scube.toStr() << " at 2\n";
	for  each (auto a in scube.fromCenterInRange(2))
	{
		debug << a.toStr() << "\n";
	}
	debug << "\nTest 6: make diagonals from-> " << scube.toStr() << "\n";
	for (int i = 0; i <= Lefttop; ++i)
	{
		debug << scube.diagonal((CubeDirections)i).toStr() << "\n";
	}
	return a.exec();
}
