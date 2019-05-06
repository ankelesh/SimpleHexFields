#pragma once
#include <qwidget.h>
#include "HexfieldWidget.h"

class simpleDemo : public QWidget
{
	Q_OBJECT
protected:
	QHBoxLayout * mainLayout;
	QVBoxLayout * hfieldLayout;
	QVBoxLayout * buttons;
	WeightedMapWidget * hexfield;
	QLabel * startcoord;
	QLabel * finishcoord;
	QCheckBox * autosteps;
	QCheckBox * arrows;
	QPushButton * stepbutton;
	QLabel * cost_lim_announce;
	QSpinBox * cost_limit;
	QLabel * path_cost_info;
	QTimer * timer;
	ICube start;
	QList<ICube> path;
	hexagonDrawOptions lineOptions;
	bool ColorizeHexes, ShowWeights, CostLimitation;
	int Delay;
public:
	simpleDemo(std::string fname, const int size = 50,
		const int delay = 1000, const hexagonDrawOptions lo =HEX_DEFAULTS,
		const bool colorizehexes=true, const bool showweights = true,
		const bool cost_limitation = false,
		const QSize widgetsize=QSize(500,500),
		QWidget * parent = Q_NULLPTR);
	virtual UniPFResult findPath(const ICube & start, const ICube & finish,
		const int max_cost = 0) = 0;
public slots:
	void onClick(const bool left, const int x, const int y);
	void step_asked();
};
class BreadthDemo : public simpleDemo
{
public:
	BreadthDemo(std::string fname, const int size = 50,
		const int delay = 1000, const hexagonDrawOptions lo = HEX_DEFAULTS,
		const QSize widgetsize = QSize(500, 500),
		QWidget * parent = Q_NULLPTR);
	UniPFResult findPath(const ICube & start, const ICube & finish,
		const int max_cost = 0);
};
class FirstMatchDemo : public simpleDemo
{
public:
	FirstMatchDemo(std::string fname, const int size = 50,
		const int delay = 1000, const hexagonDrawOptions lo = HEX_DEFAULTS,
		const QSize widgetsize = QSize(500, 500),
		QWidget * parent = Q_NULLPTR);
	UniPFResult findPath(const ICube & start, const ICube & finish,
		const int max_cost = 0);
};
class DijkstraDemo : public simpleDemo
{
public:
	DijkstraDemo(std::string fname, const int size = 50,
		const int delay = 1000, const hexagonDrawOptions lo = HEX_DEFAULTS,
		const QSize widgetsize = QSize(500, 500),
		QWidget * parent = Q_NULLPTR);
	UniPFResult findPath(const ICube & start, const ICube & finish,
		const int max_cost = 0);
};
class AStarDemo : public simpleDemo
{
public:
	AStarDemo(std::string fname, const int size = 50,
		const int delay = 1000, const hexagonDrawOptions lo = HEX_DEFAULTS,
		const QSize widgetsize = QSize(500, 500),
		QWidget * parent = Q_NULLPTR);
	UniPFResult findPath(const ICube & start, const ICube & finish,
		const int max_cost = 0);
};
class LimitedDijkstraDemo : public simpleDemo
{
public:
	LimitedDijkstraDemo(std::string fname, const int size = 50,
		const int delay = 1000, const hexagonDrawOptions lo = HEX_DEFAULTS,
		const QSize widgetsize = QSize(500, 500),
		QWidget * parent = Q_NULLPTR);
	UniPFResult findPath(const ICube & start, const ICube & finish,
		const int max_cost = 0);
};
class LimitedAStarDemo : public simpleDemo
{
public:
	LimitedAStarDemo(std::string fname, const int size = 50,
		const int delay = 1000, const hexagonDrawOptions lo = HEX_DEFAULTS,
		const QSize widgetsize = QSize(500, 500),
		QWidget * parent = Q_NULLPTR);
	UniPFResult findPath(const ICube & start, const ICube & finish,
		const int max_cost = 0);
};