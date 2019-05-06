#pragma once
#include <HexfieldWidget.h>


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
	WeightedMapWidget * hexfield;
	QPushButton * stepButton;
	hexagonDrawOptions lineopts;
	ICube start;
	QList<ICube> path;
	QTimer * timer;
public:
	PathfindTestGui(QList<ICube> hlist, QSize widgetsz, hexagonDrawOptions lo, QWidget * parent = Q_NULLPTR);
	PathfindTestGui(std::string & fname, QSize widgetsz, hexagonDrawOptions lo, QWidget * parent = Q_NULLPTR);
public slots:
	void step_asked();
	void click_received(bool left, int x, int y);
};