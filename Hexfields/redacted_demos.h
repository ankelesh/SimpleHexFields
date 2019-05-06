#pragma once
#include <qwidget.h>
#include "HexfieldWidget.h"

class redactableDemo : public QWidget
{
	Q_OBJECT
protected:
	QHBoxLayout * mainLayout;
	QHBoxLayout * radiocheckpanel;
	QVBoxLayout * buttons;
	WeightedMapWidget * hexfield;
	QLabel * startcoord;
	QLabel * finishcoord;
	QLabel * weightannounce;
	QPushButton * stepbutton;
	QCheckBox * autosteps;
	QCheckBox * arrows;
	QRadioButton * redactMode;
	QRadioButton * setupMode;
	QSpinBox * weightSpin;
	QTimer * timer;
	ICube start;
	ICube finish;
	QList<ICube> path;
	hexagonDrawOptions lineOptions;
	bool ColorizeHexes, ShowWeights, CostLimitation;
	int Delay;
	void setText(const ICube & a, const ICube & b);
	void setText(const ICube & a);
	void makePathfind();
public:
	redactableDemo(std::string fname, const int size = 50,
		const int delay = 1000, const hexagonDrawOptions lo = HEX_DEFAULTS,
		const bool colorizehexes = true, const bool showweights = true,
		const bool cost_limitation = false,
		const QSize widgetsize = QSize(500, 500),
		QWidget * parent = Q_NULLPTR);
	virtual UniPFResult findPath(const ICube & start, const ICube & finish,
		const int max_cost = 0) = 0;
public slots:
	void onClick(const bool left, const int x, const int y);
	void step_asked();
};
class DijkstraRedactableDemo : public redactableDemo
{
public:
	DijkstraRedactableDemo(std::string fname, const int size = 50,
		const int delay = 1000, const hexagonDrawOptions lo = HEX_DEFAULTS,
		const QSize widgetsize = QSize(500, 500),
		QWidget * parent = Q_NULLPTR);
	virtual UniPFResult findPath(const ICube & start, const ICube & finish,
		const int max_cost = 0);
};
class AStarRedactableDemo : public redactableDemo
{
public:
	AStarRedactableDemo(std::string fname, const int size = 50,
		const int delay = 1000, const hexagonDrawOptions lo = HEX_DEFAULTS,
		const QSize widgetsize = QSize(500, 500),
		QWidget * parent = Q_NULLPTR);
	virtual UniPFResult findPath(const ICube & start, const ICube & finish,
		const int max_cost = 0);
};

