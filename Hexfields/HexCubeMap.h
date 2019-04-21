#pragma once
#include <CubeCoords.h>
#include <HexDrawingTools.h>

using namespace HexCoords;
typedef QMap<ICube, Hexagon> hexmap;
typedef QMap<QString, hexagonDrawOptions> optmap;
class HexCubeMap
{
private:
	hexmap core_storage;
	QPoint coord_start;
	int hex_size;
	bool pointy;
public:
	HexCubeMap();
	HexCubeMap(const ICube & start, const ICube & finish, const int size,
		QPoint offset, hexagonDrawOptions & defaults, bool pnty, QString & def_tag);
	HexCubeMap(QList<ICube> & coordlist, const int size, QPoint offset, hexagonDrawOptions & defaults,
		bool pnty, QString & def_tag);
	QMap<ICube, Hexagon>::iterator begin() { return core_storage.begin(); };
	QMap<ICube, Hexagon>::iterator end() { return core_storage.end(); };
	Hexagon & operator[](const ICube & key) { return core_storage[key]; };
	bool contains(const ICube & key) { return core_storage.contains(key); };
	void insert(const ICube & key, const Hexagon & value)
	{
		if (!core_storage.contains(key)) 
			core_storage.insert(key, value);
	};
	void setOptsForCoordList(const QList<ICube> & coordlist, hexagonDrawOptions & opts);
	void setOptsForCoordList(const std::vector<ICube> & coordlist, hexagonDrawOptions & opts);
	void setOptsForTag(const QString & tag, hexagonDrawOptions & opts);
	void setOptsForOne(const ICube & coord, const hexagonDrawOptions & opts);
	void dropOpts(hexagonDrawOptions & opts);
};

