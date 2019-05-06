#include "stdafx.h"
#include "redacted_demos.h"

redactableDemo::redactableDemo(std::string fname, const int size,
	const int delay, const hexagonDrawOptions lo,
	const bool colorizehexes, const bool showweights,
	const bool cost_limitation,
	const QSize widgetsize,
	QWidget * parent)
	:	QWidget(parent), mainLayout(new QHBoxLayout(this)),
	radiocheckpanel(new QHBoxLayout(this)), buttons(new QVBoxLayout(this))
	, hexfield(new WeightedMapWidget(fname, size, widgetsize, colorizehexes,this))
	, startcoord(new QLabel(this)), finishcoord(new QLabel(this)),
	weightannounce(new QLabel(this)), stepbutton(new QPushButton(this)),
	autosteps(new QCheckBox("Autostep",this)), arrows(new QCheckBox("Arrows",this)),
	redactMode(new QRadioButton(this)), setupMode(new QRadioButton(this)),
	weightSpin(new QSpinBox(this)), timer(new QTimer(this)),
	start(), path(), lineOptions(lo), ColorizeHexes(colorizehexes),
	ShowWeights(showweights), CostLimitation(cost_limitation), Delay(delay)
{
	if (ColorizeHexes) {
		for (int i = 0; i < colorranges::intervalQuantity; ++i)
		{
			hexfield->addColorInterval(*(colorranges::normalLayout[i]));
		}
		hexfield->setupColors();
	}
	hexfield->setTestWidgetView(showweights);
	this->setLayout(mainLayout);
	mainLayout->addWidget(hexfield);
	mainLayout->addLayout(buttons);
	buttons->addWidget(startcoord);
	buttons->addWidget(finishcoord);
	buttons->addWidget(weightannounce);
	buttons->addWidget(weightSpin);
	buttons->addLayout(radiocheckpanel);
	radiocheckpanel->addWidget(redactMode);
	radiocheckpanel->addWidget(setupMode);
	radiocheckpanel->addWidget(autosteps);
	radiocheckpanel->addWidget(arrows);
	buttons->addWidget(stepbutton);
	start = hexfield->getMap().begin().key();
	finish = (hexfield->getMap().end()-1).key();
	setText(start, finish);
	weightannounce->setText("Enter the weight for redactor below");
	weightSpin->setMinimum(0);
	weightSpin->setMaximum(200);
	setupMode->setChecked(true);
	setupMode->setText("Setup");
	redactMode->setText("Redactor");
	autosteps->setChecked(true);
	stepbutton->setText("Step");
	buttons->addStretch(1);
	hexfield->setTestWidgetView();
	this->setFont(QFont("Times new Roman", 18, 18));
	hexfield->setFont(QFont("Arial", 8, 8));
	QObject::connect(stepbutton, &QPushButton::clicked, this, &redactableDemo::step_asked);
	QObject::connect(timer, &QTimer::timeout, this, &redactableDemo::step_asked);
	QObject::connect(hexfield, &WeightedMapWidget::clicked, this, &redactableDemo::onClick);
}
void redactableDemo::setText(const ICube & a, const ICube & b)
{
	startcoord->setText("Current start coordinate is (" +
		QString::fromStdString(a.toStr()) + ")");
	finishcoord->setText("Current finish coordinate is (" +
		QString::fromStdString(b.toStr()) + ")");
}
void redactableDemo::setText(const ICube & a)
{
	startcoord->setText("Current start coordinate is (" +
		QString::fromStdString(a.toStr()) + ")");
}
void redactableDemo::onClick(const bool left, const int x, const int y)
{
	QPoint offset = hexfield->getOffset();
	ICube temp;
	temp.fromPixel(x, y, 50, offset.x(), offset.y(), true);
	if (redactMode->isChecked())
	{
		if (left)
		{
			if (hexfield->getMap().contains(temp))
			{
				hexfield->getMap().setWeightForOne(temp, weightSpin->value());
				hexfield->getMap()[temp].set_text(
					QString::fromStdString(temp.toStr()) + QString("w=") +
					QString::number(weightSpin->value()));
				hexfield->setupColorForOne(temp);
				makePathfind();
			}
		}
		else
		{
			if (hexfield->getMap().contains(temp))
			{
				hexfield->getMap().remove(temp);
				makePathfind();
			}
			else
			{
				hexfield->getMap().insert(temp, weightSpin->value());
				hexfield->getGraph().insertHeight(WeightedCoord({ temp, weightSpin->value() }));
				hexfield->getMap()[temp].set_text(
					QString::fromStdString(temp.toStr()) + QString("w=") 
					+ QString::number(weightSpin->value()));
				hexfield->setupColorForOne(temp);
				makePathfind();
			}
		}
	}
	else
	{
		if (left)
		{
			if (hexfield->getMap().contains(temp)) {
				finish = temp;
				makePathfind();
			}
		}
		else
		{
			if (hexfield->getMap().contains(temp)) {
				start = temp;
				makePathfind();
			}

		}
		setText(start, finish);
	}
}

void redactableDemo::step_asked()
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
void redactableDemo::makePathfind()
{
	hexfield->dropField();
	UniPFResult uniresult;
	uniresult = findPath(start, finish);
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
	update();
	if (autosteps->isChecked())
		timer->start(Delay);
}

DijkstraRedactableDemo::DijkstraRedactableDemo(std::string fname,
	const int size, const int delay, const hexagonDrawOptions lo,
	const QSize widgetsize, QWidget * parent)
	: redactableDemo(fname, size, delay, lo)
{}

UniPFResult DijkstraRedactableDemo::findPath(const ICube & start, const ICube & finish, const int max_cost)
{
	return hexfield->getGraph().UniformDijkstra(start, finish);
}

AStarRedactableDemo::AStarRedactableDemo(std::string fname, const int size,
	const int delay, const hexagonDrawOptions lo, 
	const QSize widgetsize, QWidget * parent)
	: redactableDemo(fname, size, delay, lo)
{
}

UniPFResult AStarRedactableDemo::findPath(const ICube & start, const ICube & finish, const int max_cost)
{
	return hexfield->getGraph().UniformAStar(start, finish);
}
