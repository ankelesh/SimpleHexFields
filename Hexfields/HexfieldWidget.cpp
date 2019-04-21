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

LineTestGui::LineTestGui(QList<ICube> hlist, int size, QSize widgetsz,
	hexagonDrawOptions lo, QWidget * parent )
	: mainLayout(new QHBoxLayout(this)),hexfield(Q_NULLPTR),coordView(new QLabel(this)), start(), lineopts(lo), hex_sz(size)
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
	:QWidget(parent), mainLayout(new QHBoxLayout(this)), hexfield(new HexfieldWidget(hlist, 50, widgetsz, this)),
	stepButton(new QPushButton("Step",this)), lineopts(lo),weightedField(), start(ICube(0,0,0)), path()
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(hexfield);
	mainLayout->addWidget(stepButton);
	hexfield->setTestWidgetView();
	auto coord = hlist.begin();
	while (coord != hlist.end())
	{
		weightedField.insert(*coord, 1);
		++coord;
	}
	HexaGraph hg(&weightedField);
	path = hg.DijkstraSearch(start, ICube(2, 0, -2)).toList();
	QObject::connect(stepButton, &QPushButton::clicked, this, &PathfindTestGui::step_asked);
	QObject::connect(hexfield, &HexfieldWidget::clicked, this, &PathfindTestGui::click_received);
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
		if (weightedField.contains(temp)) {
			HexaGraph hg(&weightedField);
			path = hg.DijkstraSearch(start, temp).toList();
			debug << "finish changed to " << start.toStr() << " and new path len is " << path.count() << "\n";
			update();
		}
		return;
	}
	ICube temp;
	temp.fromPixel(x, y, 50, offs.x(), offs.y(), true);
	if (weightedField.contains(temp))
	{
		start = temp;
	}
}