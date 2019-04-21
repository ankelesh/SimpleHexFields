#pragma once
#include <cmath>
#include <qpoint.h>

namespace HexCoords
{
	struct hexagonDrawOptions
	{
		QColor inner_color;
		QColor border_color;
		int border_width;
	};

	static hexagonDrawOptions HEX_DEFAULTS
	({
		QColor(Qt::white),
		QColor(Qt::black),
		1
		});



	class Hexagon
	{
	private:
		QPolygon polygon;
		bool pointy;
		QPoint center;
		int size;
		hexagonDrawOptions opts;
		QString text;
		QString tag;
	public: 
		Hexagon(const QPoint & center, const int & size, const int & coord_start_offset_x, const int & coord_start_offset_y
		, hexagonDrawOptions & options, const bool & pointy = true, const QString & txt = "", const QString & tg = "");
		Hexagon();
		QPolygon & getPolygon() {	return polygon;	};
		std::string printPoints() const;
		bool	isPointy() const { return pointy; }
		QPoint  getCenter() const { return center; }
		hexagonDrawOptions & get_options();
		void	set_options(const hexagonDrawOptions &);
		QPoint  get_text_place();
		QString get_text();
		void	set_text(const QString &);
		QString get_tag() const { return tag; };
		void set_tag(const QString & qs) { tag = qs; };
		Hexagon & operator=(const Hexagon & hg);
	};
	QPoint	find_corner(const QPoint & center, const int & size,
				const int & corner_num,const bool & pointy = true);
	int		hex_width(const int & size,const bool & pointy);
	int		hex_height(const int & size,const bool & pointy);



}
