#pragma once
#include <qwidget.h>
#include <preset_demos.h>
#include <redacted_demos.h>
#include <HexRedactor.h>

enum DemoKinds{Breadth, First_match, 
	Dijkstra, Dijkstra_limited,
	A_star, A_star_limited,
	Dijkstra_redactable, A_star_redactable};
static hexagonDrawOptions lineopts =
	hexagonDrawOptions({ QColor(Qt::red), QColor(Qt::black), 1 });
class Demo :
	public QWidget
{
	QTabWidget * mainspace;
	QHBoxLayout * lay;
	QVector<simpleDemo *> preset_demos;
	QVector<redactableDemo *> redactable_demos;
	HexRedactor * hredactor;
public:
	Demo(QWidget * parent = Q_NULLPTR);
};

