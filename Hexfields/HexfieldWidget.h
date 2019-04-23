#pragma once

#include <QtWidgets/QWidget>
#include "HexCubeMap.h"
#include "HexaGraph.h"



using namespace HexCoords;

class HexfieldWidget : public QWidget
{
	Q_OBJECT
protected:
	HexCubeMap map;
	int hexsize;
	QMap<QString, hexagonDrawOptions> drawOptions;
	QPoint offset;
public:
	HexfieldWidget(hexmap & hmap,const int & size, const QSize & widgetSize,QWidget *parent = Q_NULLPTR);
	HexfieldWidget(QList<ICube> hlist,const int & size, const QSize & widgetSize, QWidget *parent = Q_NULLPTR);
	void setTestWidgetView();
	void drawLine(ICube start, ICube finish, hexagonDrawOptions & options);
	void drawOneHex(ICube at, hexagonDrawOptions & options);
	void paintEvent(QPaintEvent * qpe);
	void mousePressEvent(QMouseEvent * qme);
	QPoint getOffset() { return offset; }
	void dropField() { map.dropOpts(HEX_DEFAULTS); }
signals:
	void clicked(bool left, int x, int y);
};
class WeightedMapWidget : public QWidget
{
	Q_OBJECT
protected:
	int hexsize;
	QPoint offset; 
	HexWeightedMap map;
public:
	WeightedMapWidget(weightedmap & wm, const int & size, const QSize & widgetSize, QWidget *parent = Q_NULLPTR);
	WeightedMapWidget(std::string & fname, const int & size, const QSize & widgetSize, QWidget *parent = Q_NULLPTR);
	void setTestWidgetView();
	void drawLine(ICube start, ICube finish, hexagonDrawOptions & options);
	void drawOneHex(ICube at, hexagonDrawOptions & options);
	void paintEvent(QPaintEvent * qpe);
	void mousePressEvent(QMouseEvent * qme);
	QPoint getOffset() { return offset; }
	void dropField() { map.dropOpts(HEX_DEFAULTS); }
	HexaGraph & getGraph() { return map.getGraph(); };
	bool hasHex(const ICube & ic) { return map.contains(ic); };
	HexWeightedMap & getMap() {
		return map;
	};
signals:
	void clicked(bool left, int x, int y);
};

