#pragma once

#include <QtWidgets/QWidget>
#include "HexCubeMap.h"
#include "HexaGraph.h"



using namespace HexCoords;

class HexfieldWidget : public QWidget
{
	Q_OBJECT
private:
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
class LineTestGui : public QWidget
{
	Q_OBJECT
private:
	QHBoxLayout * mainLayout;
	HexfieldWidget * hexfield;
	QLabel * coordView;
	ICube start;
	hexagonDrawOptions lineopts;
	int hex_sz;
public:
	LineTestGui(QList<ICube> hlist, int size, QSize widgetsz, hexagonDrawOptions lo, QWidget * parent = Q_NULLPTR);
public slots:
	void click_received(bool left, int x, int y);
};
class PathfindTestGui : public QWidget
{
	Q_OBJECT
private:
	QHBoxLayout * mainLayout;
	HexfieldWidget * hexfield;
	QPushButton * stepButton;
	hexagonDrawOptions lineopts;
	ICube start;
	QMap<ICube, int> weightedField;
	QList<ICube> path;
public:
	PathfindTestGui(QList<ICube> hlist, QSize widgetsz, hexagonDrawOptions lo, QWidget * parent = Q_NULLPTR);
public slots:
	void step_asked();
	void click_received(bool left, int x, int y);
};
