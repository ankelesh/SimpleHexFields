#include "stdafx.h"
#include "HexfieldWidget.h"


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
	QRect bg(QPoint(0,0), QPoint(size().width(), size().height()));
	painter.drawRect(bg);
	auto begin = map.begin();
	while (begin != map.end())
	{
		auto thisHexOpts = begin->get_options();
		painter.setBrush(QBrush(thisHexOpts.inner_color));
		painter.setPen(QPen(thisHexOpts.border_color, thisHexOpts.border_width));
		painter.drawPolygon(begin->getPolygon());
		painter.drawText(begin->get_text_place(), begin->get_text());
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
	const QSize & widgetSize, QWidget *parent)
	: QWidget(parent), hexsize(size), offset(size, size),
	map(fname, hexsize, offset)
{
	this->setMinimumSize(widgetSize);
}
void WeightedMapWidget::setTestWidgetView()
{
	auto begin = map.begin();
	while (begin != map.end())
	{
		begin->set_text(QString::fromStdString(begin.key().toStr())+ " w=" 
			+ QString::number(map.getWeight(begin.key())));
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
	QPainter painter(this);
	QRect bg(QPoint(0, 0), QPoint(size().width(), size().height()));
	painter.drawRect(bg);
	auto begin = map.begin();
	while (begin != map.end())
	{
		auto thisHexOpts = begin->get_options();
		painter.setBrush(QBrush(thisHexOpts.inner_color));
		painter.setPen(QPen(thisHexOpts.border_color, thisHexOpts.border_width));
		painter.drawPolygon(begin->getPolygon());
		painter.drawText(begin->get_text_place(), begin->get_text());
		++begin;
	}
}
void WeightedMapWidget::mousePressEvent(QMouseEvent * qme)
{
	emit clicked((qme->button() == Qt::LeftButton) ? true : false, qme->x(), qme->y());
}

