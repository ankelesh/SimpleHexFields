#include "stdafx.h"
#include "HexDrawingTools.h"
#include<sstream>

namespace HexCoords
{
	QPoint	find_corner(const QPoint & center,const int & size, 
		const int & corner_num, const bool & pointy)
	{
		float adeg = (pointy)?  60 * corner_num - 30 : 60 * corner_num;
		float arad = std::_Pi / 180 * adeg;
		return QPoint(center.x() + size * cos(arad), center.y() + size * sin(arad));
	}
	int		hex_width(const int & size,const bool & pointy)
	{
		return (pointy) ? size * sqrt(3) : hex_height(size, true);
	}
	int		hex_height(const int & size, const bool & pointy)
	{
		return (pointy) ? 2 * size : hex_width(size, true);
	}
	QPolygon makeArrow(const QPoint & center, const int size, const CubeDirections direction, const bool pointy)
	{
		
		QVector<QPoint> pnts;
		pnts.push_back(find_corner(center, size* 0.85, direction, !pointy)); //top
		QPoint bot = (find_corner(center, size * 0.6, direction, !pointy));
		//QPoint root = (find_corner(center, size * 0.2, direction, !pointy));
		CubeDirections ldir = (CubeDirections)
			((direction + 2 > 5) ? direction - 4 : direction + 2);
		CubeDirections rdir = (CubeDirections)
			((direction -1 < 0) ? direction +5 : direction -1);
		pnts.push_back(find_corner(center, size* 0.85, direction, !pointy)); //top
		pnts.push_back(find_corner(bot, size *0.3, rdir, pointy)); //right
		//pnts.push_back(find_corner(bot, size * 0.05, rdir, pointy));
		//pnts.push_back(find_corner(root, size * 0.05, rdir, pointy)); //bottom rect
		//pnts.push_back(find_corner(root, size * 0.05, ldir, pointy));
		//pnts.push_back(find_corner(bot, size * 0.05, ldir, pointy));
		pnts.push_back(find_corner(bot, size*0.3, ldir, pointy)); //left
		return QPolygon(pnts);
	}
	Hexagon::Hexagon(const QPoint & Center, const int & Size, const int & coord_start_offset_x,
		const int & coord_start_offset_y
		, hexagonDrawOptions & options, const bool & Pointy, const QString & txt, const QString & tg)
		: polygon(), pointy(Pointy), center(Center),size(Size), opts(options), text(txt), tag(tg)
		,direction(None)
	{
		center.rx() += coord_start_offset_x;
		center.ry() += coord_start_offset_y;
		QVector<QPoint> points;
		points.reserve(6);
		for (int i = 0; i < 6; ++i)
		{
			points.push_back(find_corner(center, size, i, pointy));
		}
		polygon = QPolygon(points);
	}
	std::string Hexagon::printPoints() const
	{
		std::ostringstream sout;
		sout << "Hexagon at " << center.x() << ' ' << center.y() <<
			" size: " << size << " and it's ";
		if (pointy)
			sout << "pointy";
		else
			sout << "flat";
		sout << "\nIts points: ";
		for (int i = 0; i < 6; ++i)
		{
			QPoint qp = find_corner(center, size, i, pointy);
			sout << qp.x() << ' ' << qp.y() << "| ";
		}
		return sout.str();
	}
	Hexagon::Hexagon() : polygon(), pointy(true), center(QPoint(0, 0)), size(1), opts(HEX_DEFAULTS)
	{
		opts = hexagonDrawOptions(HEX_DEFAULTS);
		text = "";
	}
	hexagonDrawOptions & Hexagon::get_options()
	{
		return opts;
	}
	void	Hexagon::set_options(const hexagonDrawOptions & op)
	{
		opts = op;
	}
	QPoint  Hexagon::get_text_place()
	{
		return QPoint(center.x() - size / 2, center.y()- size * 0.5);
	}
	QString Hexagon::get_text()
	{
		return text;
	}
	void	Hexagon::set_text(const QString & st)
	{
		text = st; text_changed = true;
	}
	Hexagon & Hexagon::operator=(const Hexagon & hg)
	{
		polygon = hg.polygon;
		pointy = hg.pointy;
		center = hg.center;
		size = hg.size;
		opts = hg.opts;
		text = hg.text;
		return *this;
	}
}