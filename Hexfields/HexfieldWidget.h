#pragma once

#include <QtWidgets/QWidget>
#include "HexCubeMap.h"
#include "HexaGraph.h"



using namespace HexCoords;

class colorInterval
{
private:
	int higher;
	int lower;
	hexagonDrawOptions opts;
public:
	colorInterval(const int h = 0, const int l = 0, 
		const hexagonDrawOptions & o = HEX_DEFAULTS);
	bool inRange(const int) const;
	hexagonDrawOptions getOpts() const;
	void setOpts(const hexagonDrawOptions &);
	void setInterval(const int, const int);
};



class HexfieldWidget : public QWidget
{
	Q_OBJECT
protected:
	HexCubeMap map;
	int hexsize;
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
	void dropField() { map.dropOpts(HEX_DEFAULTS); map.dropDirs(); }
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
	bool colorizeWeights;
	QVector<colorInterval> ints;
public:
	WeightedMapWidget(weightedmap & wm, const int & size, const QSize & widgetSize, QWidget *parent = Q_NULLPTR);
	WeightedMapWidget(std::string & fname, const int & size, const QSize & widgetSize, bool cw = false, QWidget *parent = Q_NULLPTR);
	void setTestWidgetView(bool shw = true);
	void drawLine(ICube start, ICube finish, hexagonDrawOptions & options);
	void drawOneHex(ICube at, hexagonDrawOptions & options);
	void paintEvent(QPaintEvent * qpe);
	void mousePressEvent(QMouseEvent * qme);
	QPoint getOffset() { return offset; }
	int getHexSize() { return hexsize; }
	void dropField() { 
		if (colorizeWeights) setupColors(); 
		else map.dropOpts(HEX_DEFAULTS);
		map.dropDirs(); }
	HexaGraph & getGraph() { return map.getGraph(); };
	bool hasHex(const ICube & ic) { return map.contains(ic); };
	void addColorInterval(const colorInterval & ci) { ints.push_back(ci); };
	void setupColors();
	void setupColorForOne(const ICube & coord);
	void setColors(bool cw) { colorizeWeights = cw; };
	void dropInts() { ints.clear(); };
	HexWeightedMap & getMap() {
		return map;
	};
signals:
	void clicked(bool left, int x, int y);
};

namespace colorranges {
	static colorInterval town(0, 2, hexagonDrawOptions({
		QColor(74, 255, 38), QColor(96, 78, 33), 10 }));
	static colorInterval goodRoad(2, 5, hexagonDrawOptions({
			Qt::lightGray,Qt::black, 1 }));
	static colorInterval bridge(5, 6, hexagonDrawOptions({
		QColor(81, 87, 127), Qt::black, 1
		}));
	static colorInterval badRoad(5, 10, hexagonDrawOptions({
		QColor(229,164,59),	Qt::black, 1 }));
	static colorInterval grass(10, 15, hexagonDrawOptions(
		{ QColor(72, 135,39), Qt::black, 1 }));
	static colorInterval forest(15, 30, hexagonDrawOptions(
		{ QColor(41, 73,23), Qt::black, 1 }));
	static colorInterval hill(30, 60, hexagonDrawOptions(
		{ QColor(77,99,6), Qt::black, 1 }));
	static colorInterval sand(60, 80, hexagonDrawOptions(
		{ QColor(201, 184, 108), Qt::black, 1 }));
	static colorInterval mountain(80, 100, hexagonDrawOptions(
		{ Qt::gray, Qt::black, 1 }));
	static colorInterval river(100, 1000, hexagonDrawOptions(
		{ QColor(104,160,204), Qt::black, 1 }));
	static colorInterval* normalLayout[]{ &town, 
		&goodRoad,&bridge, &badRoad, &grass, &forest, &hill, &sand, &mountain, &river
	};
	static const int intervalQuantity = 10;
	static QColor addColors(QColor & color1, QColor & color2, double r = 0.5) {
		return QColor((color1.red() * (1 - r) + color2.red() * r),
			(color1.green()* (1 - r) + color2.green() *r),
			(color1.blue() *(1 - r) + color2.blue() * r),
			255);
	};
}
