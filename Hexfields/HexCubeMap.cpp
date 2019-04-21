#include "stdafx.h"
#include "HexCubeMap.h"


HexCubeMap::HexCubeMap() : core_storage(), coord_start(QPoint(40,40)), hex_size(50), pointy(true)
{
}
HexCubeMap::HexCubeMap(const ICube & start, const ICube & finish, const int size,
	QPoint offset, hexagonDrawOptions & defaults, bool pnty, QString & def_tag)
	: core_storage(), coord_start(offset), hex_size(size), pointy(pnty)
{
	ICube summ = start + finish;
	ICube middle(summ.getX() / 2, summ.getY() / 2, summ.getZ() / 2);
	auto hexlist = middle.fromCenterInRange(abs(start.getX() - middle.getX()));
	auto begin = hexlist.begin();
	while (begin != hexlist.end())
	{
		auto pix = begin->toPixel(hex_size, pointy);
		core_storage.insert(*begin, Hexagon(
			QPoint(pix.first, pix.second),hex_size,offset.x(), offset.y(),
			defaults, pointy, "", def_tag
		));
		++begin;
	}
}
HexCubeMap::HexCubeMap(QList<ICube> & coordlist, const int size,  QPoint offset, hexagonDrawOptions & defaults,
	bool pnty, QString & def_tag) : core_storage(), coord_start(offset), hex_size(size), pointy(pnty)
{
	auto begin = coordlist.begin();
	while (begin != coordlist.end())
	{
		auto pix = begin->toPixel(size, pointy);
		core_storage.insert(*begin, Hexagon(
			QPoint(pix.first, pix.second), hex_size,offset.x(), offset.y(),
			defaults, pointy, "", def_tag
		));
		++begin;
	}
}
void HexCubeMap::setOptsForCoordList(const QList<ICube> & coordlist, hexagonDrawOptions & opts)
{
	auto begin = coordlist.begin();
	while (begin != coordlist.end())
	{
		if (core_storage.contains(*begin))
		{
			core_storage[*begin].set_options(opts);
		}
		++begin;
	}
}
void HexCubeMap::setOptsForCoordList(const std::vector<ICube> & coordlist, hexagonDrawOptions & opts)
{
	auto begin = coordlist.begin();
	while (begin != coordlist.end())
	{
		if (core_storage.contains(*begin))
		{
			core_storage[*begin].set_options(opts);
		}
		++begin;
	}
}
void HexCubeMap::setOptsForTag(const QString & tag, hexagonDrawOptions & opts)
{
	auto begin = core_storage.begin();
	while (begin != core_storage.end())
	{
		if (begin->get_tag() == tag)
		{
			begin->set_options(opts);
		}
		++begin;
	}
}
void HexCubeMap::setOptsForOne(const ICube & coord, const hexagonDrawOptions & opts)
{
	if (core_storage.contains(coord))
	{
		core_storage[coord].set_options(opts);
	}
}
void HexCubeMap::dropOpts(hexagonDrawOptions & opts)
{
	auto begin = core_storage.begin();
	while (begin != core_storage.end())
	{
		begin->set_options(opts);
		++begin;
	}
}

