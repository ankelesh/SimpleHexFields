#include "stdafx.h"
#include "TestsGui.h"


LineTestGui::LineTestGui(QList<ICube> hlist, int size, QSize widgetsz,
	hexagonDrawOptions lo, QWidget * parent)
	: mainLayout(new QHBoxLayout(this)), hexfield(Q_NULLPTR), coordView(new QLabel(this)), start(), lineopts(lo), hex_sz(size)
{
	this->setLayout(mainLayout);
	hexfield = new HexfieldWidget(hlist, size, widgetsz, this);
	mainLayout->addWidget(hexfield);
	mainLayout->addWidget(coordView);
	start = hlist.first();
	hexfield->setTestWidgetView();
	QObject::connect(hexfield, &HexfieldWidget::clicked, this, &LineTestGui::click_received);
}
void LineTestGui::click_received(bool left, int x, int y)
{
	QPoint offs = hexfield->getOffset();
	if (left)
	{
		hexfield->dropField();
		ICube temp;
		temp.fromPixel(x, y, hex_sz, offs.x(), offs.y(), true);
		coordView->setText(QString::fromStdString(temp.toStr()) + QString::number(x) + ' ' + QString::number(y));
		hexfield->drawLine(start, temp, lineopts);
		update();
		return;
	}
	start.fromPixel(x, y, hex_sz, offs.x(), offs.y(), true);
}
PathfindTestGui::PathfindTestGui(QList<ICube> hlist, QSize widgetsz, hexagonDrawOptions lo, QWidget * parent)
	:QWidget(parent), mainLayout(new QHBoxLayout(this)), hexfield(),
	stepButton(new QPushButton("Step", this)), lineopts(lo), start(ICube(0, 0, 0)), path()
{
	weightedmap temp;
	auto begin = hlist.begin();
	while (begin != hlist.end())
	{
		temp.insert(*begin, 1);
		++begin;
	}
	hexfield = new WeightedMapWidget(temp, 50, widgetsz, this);
	this->setLayout(mainLayout);
	mainLayout->addWidget(hexfield);
	mainLayout->addWidget(stepButton);
	hexfield->setTestWidgetView();
	path = hexfield->getGraph().DijkstraSearch(start, ICube(2, 0, -2)).toList();
	QObject::connect(stepButton, &QPushButton::clicked, this, &PathfindTestGui::step_asked);
	QObject::connect(hexfield, &WeightedMapWidget::clicked, this, &PathfindTestGui::click_received);
}
PathfindTestGui::PathfindTestGui(std::string & fname, QSize widgetsz, hexagonDrawOptions lo, QWidget * parent)
	:QWidget(parent), mainLayout(new QHBoxLayout(this)), hexfield(),
	stepButton(new QPushButton("Step", this)), lineopts(lo), start(ICube(0, 0, 0)), path()
{
	hexfield = new WeightedMapWidget(fname, 50, widgetsz, this);
	this->setLayout(mainLayout);
	mainLayout->addWidget(hexfield);
	mainLayout->addWidget(stepButton);
	hexfield->setTestWidgetView();
	path = hexfield->getGraph().DijkstraSearch(start, ICube(2, 0, -2)).toList();
	QObject::connect(stepButton, &QPushButton::clicked, this, &PathfindTestGui::step_asked);
	QObject::connect(hexfield, &WeightedMapWidget::clicked, this, &PathfindTestGui::click_received);
}
void PathfindTestGui::step_asked()
{
	if (!path.isEmpty())
	{
		hexfield->drawOneHex(path.takeFirst(), lineopts);
		hexfield->update();
	}
}
void PathfindTestGui::click_received(bool left, int x, int y)
{
	QPoint offs = hexfield->getOffset();
	if (left)
	{
		hexfield->dropField();
		ICube temp;
		temp.fromPixel(x, y, 50, offs.x(), offs.y(), true);
		path = hexfield->getGraph().DijkstraSearch(start, temp).toList();
		update();
		return;
	}
	ICube temp;
	temp.fromPixel(x, y, 50, offs.x(), offs.y(), true);
	if (hexfield->hasHex(temp))
		start = temp;
}