#include "stdafx.h"
#include "preset_demos.h"


simpleDemo::simpleDemo(std::string fname, const int size,
	const int delay, const hexagonDrawOptions lo,
	const bool colorizehexes, const bool showweights,
	const bool cost_limitation,
	const QSize widgetsize,
	QWidget * parent) : QWidget(parent),
	mainLayout(new QHBoxLayout(this)), hfieldLayout(new QVBoxLayout(this)),
	buttons(new QVBoxLayout(this)),
	hexfield(new WeightedMapWidget(fname, size, widgetsize, colorizehexes, this)),
	startcoord(new QLabel(this)), finishcoord(new QLabel(this)),
	autosteps(new QCheckBox("Autosteps", this)),
	arrows(new QCheckBox("Arrows", this)), stepbutton(new QPushButton("step", this))
	,cost_lim_announce(), cost_limit(), path_cost_info()
	, timer(new QTimer(this)), start(), path(), lineOptions(lo),
	ColorizeHexes(colorizehexes), ShowWeights(showweights)
	,CostLimitation(cost_limitation), Delay(delay)
{
	if (ColorizeHexes) {
		for (int i = 0; i < colorranges::intervalQuantity; ++i)
		{
			hexfield->addColorInterval(*(colorranges::normalLayout[i]));
		}
		hexfield->setupColors();
	}
	hexfield->setTestWidgetView(showweights);

	QSizePolicy isp(QSizePolicy::Maximum, QSizePolicy::Maximum);
	this->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	this->setLayout(mainLayout);
	this->setFont(QFont("Times new Roman", 18, 18));
	hexfield->setFont(QFont("Arial", 8, 8));
	mainLayout->addLayout(hfieldLayout);
	hfieldLayout->addWidget(hexfield);
	mainLayout->addLayout(buttons);
	buttons->addWidget(startcoord);
	buttons->addWidget(finishcoord);
	buttons->addWidget(autosteps);
	buttons->addWidget(arrows);
	startcoord->setSizePolicy(isp);
	finishcoord->setSizePolicy(isp);
	autosteps->setSizePolicy(isp);
	arrows->setSizePolicy(isp);
	if (cost_limitation)
	{
		cost_lim_announce = new QLabel("Set cost limitation", this);
		cost_limit = new QSpinBox(this);
		path_cost_info = new QLabel(this);
		cost_limit->setMaximum(10000);
		cost_limit->setMinimum(0);
		buttons->addWidget(cost_lim_announce);
		cost_lim_announce->setSizePolicy(isp);
		buttons->addWidget(cost_limit);
		cost_limit->setSizePolicy(isp);
		buttons->addWidget(path_cost_info);
		path_cost_info->setSizePolicy(isp);
		path_cost_info->setText("Select hex for start");
	}
	buttons->addStretch(1);
	hexfield->update();
	buttons->addWidget(stepbutton);
	stepbutton->setSizePolicy(isp);

	start = (hexfield->getMap().begin().key());
	startcoord->setText("Current start coordinate: ("
		+ QString::fromStdString(start.toStr()) + ")");
	finishcoord->setText("Please select finish coordinate");
	QObject::connect(stepbutton, &QPushButton::clicked, this, &simpleDemo::step_asked);
	QObject::connect(hexfield, &WeightedMapWidget::clicked, this, &simpleDemo::onClick);
	QObject::connect(timer, &QTimer::timeout, this, &simpleDemo::step_asked);
}
void simpleDemo::onClick(bool left, const int x, const int y)
{
	QPoint offs = hexfield->getOffset();
	if (left)
	{
		hexfield->dropField();
		ICube temp;
		temp.fromPixel(x, y, hexfield->getHexSize(), offs.x(), offs.y(), true);
		finishcoord->setText(QString("Now finish coordinate: (") + 
			QString::fromStdString(temp.toStr()) + ")");
		UniPFResult uniresult;
		if (CostLimitation) {
			uniresult = findPath(start, temp, cost_limit->value());
		}
		else
		{
			uniresult = findPath(start, temp);
		}
		debug << showResults(uniresult) << " with coords " <<
			start.toStr() << temp.toStr();
		if (!uniresult->success)
			return;
		path = uniresult->path.toList();
		auto begin = uniresult->came_from.begin();
		while (begin != uniresult->came_from.end())
		{
			ICube from = *begin;
			ICube to = begin.key();
			if (arrows->isChecked()) {
				CubeDirections dir = directionTo(to, from);
				hexfield->getMap().setDirectionForOne(to, dir);
			}
			++begin;
		}
		if (ShowWeights) {
			auto beg = uniresult->cost_so_far.begin();
			while (beg != uniresult->cost_so_far.end())
			{

				hexfield->getMap()[beg.key()].set_text(
					QString::fromStdString(beg.key().toStr()) + "\nw=" +
					QString::number(hexfield->getMap().getWeight(beg.key()))
					+ " c=" + QString::number(*beg)
				);
				++beg;
			}
		}
		if (CostLimitation)
		{
			path_cost_info->setText("This path costs " +
				QString::number(uniresult->cost_so_far[temp]));
		}
		update();
		if (autosteps->isChecked())
			timer->start(Delay);
		return;
	}
	ICube temp;
	temp.fromPixel(x, y, hexfield->getHexSize(), offs.x(), offs.y(), true);
	if (hexfield->hasHex(temp))
	{
		start = temp;
		startcoord->setText(QString("Current start coordinate: (")
			+ QString::fromStdString(start.toStr()) + ")");
	}
}
void simpleDemo::step_asked()
{
	if (!path.isEmpty())
	{
		hexagonDrawOptions lo = hexfield->getMap()[path.first()].get_options();
		lo.inner_color = colorranges::addColors(
			lineOptions.inner_color, lo.inner_color, 0.4);
		hexfield->drawOneHex(path.takeFirst(), lo);
		hexfield->update();
		if (autosteps->isChecked()) {
			timer->start(Delay);
		}
	}
}
BreadthDemo::BreadthDemo(std::string fname, const int size,
	const int delay, const hexagonDrawOptions lo,
	const QSize widgetsize,
	QWidget * parent)
	: simpleDemo(fname, size, delay, lo, false, false, false,
		widgetsize, parent) {};
UniPFResult BreadthDemo::findPath(const ICube & start, const ICube & finish,
	const int max_cost)
{
	return hexfield->getGraph().UniformBreadthFirst(start, finish);
}
FirstMatchDemo::FirstMatchDemo(std::string fname, const int size,
	const int delay, const hexagonDrawOptions lo,
	const QSize widgetsize,
	QWidget * parent)
	: simpleDemo(fname, size, delay, lo, false, false, false,
		widgetsize, parent) {};
UniPFResult FirstMatchDemo::findPath(const ICube & start, const ICube & finish,
	const int max_cost)
{
	return hexfield->getGraph().UniformBestFirst(start, finish);
}
DijkstraDemo::DijkstraDemo(std::string fname, const int size,
	const int delay, const hexagonDrawOptions lo,
	const QSize widgetsize,
	QWidget * parent)
	: simpleDemo(fname, size, delay, lo, true, true, false,
		widgetsize, parent) {};
UniPFResult DijkstraDemo::findPath(const ICube & start, const ICube & finish,
	const int max_cost )
{
	return hexfield->getGraph().UniformDijkstra(start, finish);
}
LimitedDijkstraDemo::LimitedDijkstraDemo(std::string fname, const int size,
	const int delay, const hexagonDrawOptions lo,
	const QSize widgetsize,
	QWidget * parent)
	: simpleDemo(fname, size, delay, lo, true, true, true,
		widgetsize, parent) {};
UniPFResult LimitedDijkstraDemo::findPath(const ICube & start, const ICube & finish,
	const int max_cost)
{
	return hexfield->getGraph().UniformDijkstra(start, finish, max_cost);
}
AStarDemo::AStarDemo(std::string fname, const int size,
	const int delay, const hexagonDrawOptions lo,
	const QSize widgetsize,
	QWidget * parent)
:simpleDemo(fname, size, delay, lo, true, true, false,
	widgetsize, parent) {}
UniPFResult AStarDemo::findPath(const ICube & start, const ICube & finish,
	const int max_cost)
{
	return hexfield->getGraph().UniformAStar(start, finish);
}
LimitedAStarDemo::LimitedAStarDemo(std::string fname, const int size,
	const int delay, const hexagonDrawOptions lo,
	const QSize widgetsize,
	QWidget * parent)
	:simpleDemo(fname, size, delay, lo, true, true, true,
		widgetsize, parent) {}
UniPFResult LimitedAStarDemo::findPath(const ICube & start, const ICube & finish,
	const int max_cost)
{
	return hexfield->getGraph().UniformAStar(start, finish,max_cost);
}