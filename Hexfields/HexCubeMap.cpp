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
HexCubeMap::HexCubeMap(const QList<ICube> & coordlist, const int size,  QPoint offset, hexagonDrawOptions & defaults,
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
HexWeightedMap::HexWeightedMap()
	:	HexCubeMap(),  weights_storage(), graph(&weights_storage)
{

}
HexWeightedMap::HexWeightedMap(const QList<ICube> & coordlist, QMap<ICube, int> & weights, const int size, QPoint offset, hexagonDrawOptions & defaults,
	bool pnty, QString & def_tag) :
	HexCubeMap(coordlist, size, offset, defaults, pnty, def_tag), weights_storage(weights), graph(&weights_storage)
{
}
HexWeightedMap::HexWeightedMap(const std::string & fname, const int size, QPoint & offset,
	hexagonDrawOptions & defaults, bool pnty, QString & def_tag )
	: HexCubeMap(), weights_storage(), graph(&weights_storage)
{
	coord_start = offset;
	std::ifstream fin(fname);
	if (fin.is_open())
	{
		std::string temp;
		WeightedCoord tweightcoord;
		while (!fin.eof())
		{
			std::getline(fin, temp);
			if (temp.length() < 2)
				break;
			tweightcoord = deserialize(temp);
			weights_storage.insert(tweightcoord.coord, tweightcoord.weight);
		}
	}
	auto begin = weights_storage.begin();
	while (begin != weights_storage.end())
	{
		auto pix = begin.key().toPixel(size, pointy);
		core_storage.insert(begin.key(), Hexagon(
			QPoint(pix.first, pix.second), hex_size, offset.x(), offset.y(),
			defaults, pointy, "", def_tag
		));
		++begin;
	}
	graph = HexaGraph(&weights_storage);
}
void HexWeightedMap::setWeightForOne(const ICube & coord, const int val)
{
	if (weights_storage.contains(coord))
	{
		weights_storage[coord] = val;
	}
}
void HexWeightedMap::setWeightForTag(const QString & tag, const int val)
{
	auto begin = core_storage.begin();
	while (begin != core_storage.end())
	{
		if (begin->get_tag() == tag)
		{
			weights_storage[begin.key()] = val;
		}
		++begin;
	}
}
void HexWeightedMap::setWeightForCoordList(const QList<ICube> & coordlist, const int weight)
{
	auto begin = coordlist.begin();
	while (begin != coordlist.end())
	{
		if (weights_storage.contains(*begin))
		{
			weights_storage[*begin]=weight;
		}
		++begin;
	}
}
void HexWeightedMap::setWeightForCoordList(const std::vector<ICube> & coordlist, const int weight)
{
	auto begin = coordlist.begin();
	while (begin != coordlist.end())
	{
		if (weights_storage.contains(*begin))
		{
			weights_storage[*begin] = weight;
		}
		++begin;
	}
}
void HexWeightedMap::insertWeightsIntoMap(const weightedmap & wmap)
{
	auto begin = wmap.begin();
	while (begin != wmap.end())
	{
		if (weights_storage.contains(begin.key()))
		{
			weights_storage[begin.key()] = *begin;
		}
		++begin;
	}
}
void HexWeightedMap::dropWeights()
{
	auto begin = weights_storage.begin();
	while (begin != weights_storage.end())
	{
		*begin = 1;
		++begin;
	}
}
std::string HexWeightedMap::serialize()
{
	std::ostringstream sout;
	auto begin = weights_storage.begin();
	while (begin != weights_storage.end())
	{
		sout << begin.key().toStr() << *begin << "\n";
		++begin;
	}
	return sout.str();
}
void HexWeightedMap::remove(const ICube & key)
{
	if (core_storage.contains(key))
	{
		core_storage.remove(key);
		graph.removeHeight(key);
	}
}