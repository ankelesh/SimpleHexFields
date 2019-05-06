#include "stdafx.h"
#include "HexfieldWidget.h"
#include <chrono>


HexfieldWidget::HexfieldWidget(hexmap & hmap,const int & size,const QSize & widgetSize, QWidget *parent) : 
	QWidget(parent), map(), hexsize(size)
{
	setMinimumSize(widgetSize);
}
HexfieldWidget::HexfieldWidget(QList<ICube> hlist, const int & size, const QSize & widgetSize, QWidget *parent)
	: QWidget(parent), map(), hexsize(size), offset(size, size)
{
	setMinimumSize(widgetSize);
	//int middle = (widgetSize.height() > widgetSize.width()) ? widgetSize.height() / 2 : widgetSize.width() / 2;
	map = HexCubeMap(hlist, size, offset, HEX_DEFAULTS, true, QString(""));
}
void HexfieldWidget::setTestWidgetView()
{
	auto begin = map.begin();
	while (begin != map.end())
	{
		begin->set_text(QString::fromStdString(begin.key().toStr()));
		++begin;
	}
}


void HexfieldWidget::paintEvent(QPaintEvent * qpe)
{
	QPainter painter(this);
	QRect bg(QPoint(0, 0), QPoint(size().width(), size().height()));
	painter.drawRect(bg);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	auto begin = map.begin();
	while (begin != map.end())
	{
		auto thisHexOpts = begin->get_options();
		painter.setBrush(QBrush(thisHexOpts.inner_color));
		painter.setPen(QPen(thisHexOpts.border_color, thisHexOpts.border_width));
		painter.setOpacity(1);
		painter.drawPolygon(begin->getPolygon());
		painter.drawText(QRect(begin->get_text_place(),
			begin->get_text_place() + QPoint(hexsize, hexsize )),
			begin->get_text());
		//begin->set_dir(Right);
		if (begin->get_dir() != None)
		{
			painter.setBrush(QBrush(Qt::GlobalColor::blue));
			painter.setOpacity(0.4);
			painter.drawPolygon(makeArrow(begin->getCenter(), hexsize,
				begin->get_dir(), true));
		}
		++begin;
	}

}
void HexfieldWidget::drawLine(ICube start, ICube finish, hexagonDrawOptions & options)
{
	if (map.contains(start) && map.contains(finish))
	{
		map.setOptsForCoordList(start.createLine(finish), options);
	}
}
void HexfieldWidget::drawOneHex(ICube at, hexagonDrawOptions & options)
{
	if (map.contains(at))
	{
		map.setOptsForOne(at, options);
	}
}
void HexfieldWidget::mousePressEvent(QMouseEvent * qme)
{
	emit clicked((qme->button() == Qt::LeftButton) ? true : false, qme->x(), qme->y());
}

WeightedMapWidget::WeightedMapWidget(weightedmap & wm, const int & size,
	const QSize & widgetSize, QWidget *parent)
	: QWidget(parent), hexsize(size), offset(size,size),
	map(wm.uniqueKeys(), wm, hexsize, offset)
{
	this->setMinimumSize(widgetSize);
}
WeightedMapWidget::WeightedMapWidget(std::string & fname, const int & size,
	const QSize & widgetSize,bool cw, QWidget *parent)
	: QWidget(parent), hexsize(size), offset(size, size),
	map(fname, hexsize, offset),colorizeWeights(cw)
{
	this->setMinimumSize(widgetSize);
	this->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
}
void WeightedMapWidget::setTestWidgetView(bool shw)
{
	auto begin = map.begin();
	while (begin != map.end())
	{
		if (shw) {
			begin->set_text(QString::fromStdString(begin.key().toStr()) + "\nw="
				+ QString::number(map.getWeight(begin.key())));
		}
		else
		{
			begin->set_text(QString::fromStdString(begin.key().toStr()));
		}
		++begin;
	}
}
void WeightedMapWidget::drawLine(ICube start, ICube finish, hexagonDrawOptions & options)
{
	if (map.contains(start) && map.contains(finish))
	{
		map.setOptsForCoordList(start.createLine(finish), options);
	}
}
void WeightedMapWidget::drawOneHex(ICube at, hexagonDrawOptions & options)
{
	if (map.contains(at))
	{
		map.setOptsForOne(at, options);
	}
}
void WeightedMapWidget::paintEvent(QPaintEvent * qpe)
{
	//auto start = std::chrono::high_resolution_clock::now();
	QPainter painter(this);
	QRect bg(QPoint(0, 0), QPoint(size().width(), size().height()));
	painter.drawRect(bg);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	auto begin = map.begin();
	while (begin != map.end())
	{
		//auto mapdrawstart = std::chrono::high_resolution_clock::now();
		auto thisHexOpts = begin->get_options();
		painter.setBrush(QBrush(thisHexOpts.inner_color));
		painter.setPen(QPen(thisHexOpts.border_color, thisHexOpts.border_width));
		painter.setOpacity(1);
		painter.drawPolygon(begin->getPolygon());
		//auto polydrawn = std::chrono::high_resolution_clock::now();
		if (begin->isChanged()) {
			painter.drawText(QRect(begin->get_text_place(),
				begin->get_text_place() + QPoint(hexsize*1.5, hexsize * 1.2)),
				begin->get_text());
		}
		//begin->set_dir(Righttop);
		//auto textdrawn = std::chrono::high_resolution_clock::now();
		if (begin->get_dir() != None)
		{
			painter.setBrush(QBrush(Qt::GlobalColor::blue));
			painter.setOpacity(0.4);
			painter.drawPolygon(makeArrow(begin->getCenter(), hexsize,
				begin->get_dir(), true));
		}
		//auto arrowdrawn = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double> firststop = polydrawn - mapdrawstart;
		//std::chrono::duration<double> sestop = textdrawn - mapdrawstart;
		//std::chrono::duration<double> thstop = arrowdrawn - mapdrawstart;
		//debug << "Cycle passed: " << firststop.count() << " took hex " <<
		//	sestop.count() << " took text " << thstop.count() << " took arrow\n";
		++begin;
	}
	//auto total = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> firststop = total -start;
	//debug << "Total expired: " << firststop.count();
}
void WeightedMapWidget::setupColors()
{
	if (!colorizeWeights) return;
	auto begin = map.begin();
	while (begin != map.end()) {
		auto intbeg = ints.begin();
		while (intbeg != ints.end())
		{
			if (intbeg->inRange(map.getWeight(begin.key())))
			{
				begin->set_options(intbeg->getOpts());
				break;
			}
			++intbeg;
		}
		++begin;
	}
}
void WeightedMapWidget::setupColorForOne(const ICube & coord)
{
	if (map.contains(coord))
	{
		auto intbeg = ints.begin();
		while (intbeg != ints.end())
		{
			if (intbeg->inRange(map.getWeight(coord)))
			{
				map[coord].set_options(intbeg->getOpts());
				break;
			}
			++intbeg;
		}
	}
}
void WeightedMapWidget::mousePressEvent(QMouseEvent * qme)
{
	emit clicked((qme->button() == Qt::LeftButton) ? true : false, qme->x(), qme->y());
}

colorInterval::colorInterval(const int l, const int h, const hexagonDrawOptions & o)
{
	lower = (l < h) ? l : h;
	higher = (h > l) ? h : l;
	opts = o;
}
bool colorInterval::inRange(const int value) const
{
	return (value < higher && value >= lower);
}
hexagonDrawOptions colorInterval::getOpts() const
{
	return opts;
}
void colorInterval::setOpts(const hexagonDrawOptions & o) { opts = o; }
void colorInterval::setInterval(const int h, const int l)
{
	higher = (h > l) ? h : l;
	lower = (h < l) ? h : l;
}
