#pragma once
#include "CubeCoords.h"
#include <cmath>
#include <qpoint.h>

/*
		This file contains tools helping with defining dots on the screen and draw hexagons
		contains:	
			namespace:
				HexCoords
				structs:
					hexagonDrawOptions	:	holds draw options - colors and border width
				classes:
					Hexagon				:	main class representing hexagon as it's center and precalced polygon
				functions:	
					find_corner()		:	finds corner of hex with given width and center
					hex_width()			:	calculates width using hex size value
					hex_height()		:	same
				Hexagon methods:
					getters/setters		:	text, text place, tag, options, Polygon
				constants:
					HEX_DEFAULTS		:	default settings for plain white hexagon
*/



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
		CubeDirections direction;
		bool text_changed;
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
		bool	isChanged() { return text_changed; };
		QString get_tag() const { return tag; };
		void set_tag(const QString & qs) { tag = qs; };
		void set_dir(const CubeDirections d) { direction = d; };
		CubeDirections get_dir() { return direction; }
		Hexagon & operator=(const Hexagon & hg);
	};
	QPoint	find_corner(const QPoint & center, const int & size,
				const int & corner_num,const bool & pointy = true);
	int		hex_width(const int & size,const bool & pointy);
	int		hex_height(const int & size,const bool & pointy);
	QPolygon makeArrow(const QPoint & center, const int size,
		const CubeDirections direction, const bool pointy);
	

}