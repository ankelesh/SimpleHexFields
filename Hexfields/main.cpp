#include "stdafx.h"
#include "HexfieldWidget.h"
#include "CubeCoords.h"
#include <QtWidgets/QApplication>
#include "HexaGraph.h"

int main(int argc, char *argv[])
{
	using namespace HexCoords;
	QApplication a(argc, argv);
	QList<HexCoords::ICube> plainHexField({ ICube(0,0,0), ICube(1,0,-1) ,
		ICube(2, 0,-2), ICube(3,0,-3), ICube(4,0,-4)
		, ICube(0,1,-1), ICube(1,1,-2), ICube(2,1,-3)
		,ICube(3,1,-4), ICube(-1,2,-1), ICube(0,2,-2),
		ICube(1,2,-3), ICube(2,2,-4), ICube(3,2,-5) });
	QList<HexCoords::ICube> holeyHexField({
		ICube(0,0,0),ICube(1,0,-1), ICube(2,0,-2)
		,ICube(3,0,-3),ICube(4,0,-4), ICube(1,1,-2)
		,ICube(3,1,-4),ICube(2,2,-4), ICube(3,2,-5)
		,ICube(0,2,-2),ICube(1,2,-3),ICube(0,3,-3)


		});
	hexagonDrawOptions lineopts = hexagonDrawOptions({ QColor(Qt::red), QColor(Qt::black), 1 });
	PathfindTestGui * phtg = new PathfindTestGui(holeyHexField, QSize(500, 500), lineopts);
	phtg->show();
	debug << testDijkstra(holeyHexField, ICube(0,0,0), ICube(3,2,-5));
	return a.exec();
}
