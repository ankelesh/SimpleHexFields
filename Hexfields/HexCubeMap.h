#pragma once
#include <CubeCoords.h>
#include <HexDrawingTools.h>
#include <HexaGraph.h>


using namespace HexCoords;
typedef QMap<ICube, Hexagon> hexmap;
typedef QMap<QString, hexagonDrawOptions> optmap;
typedef QMap<ICube, int> weightedmap;
class HexCubeMap
{
protected:
	hexmap core_storage;
	QPoint coord_start;
	int hex_size;
	bool pointy;
public:
	HexCubeMap();
	HexCubeMap(const ICube & start, const ICube & finish, const int size,
		QPoint offset, hexagonDrawOptions & defaults, bool pnty, QString & def_tag);
	HexCubeMap(const QList<ICube> & coordlist, const int size, QPoint offset, hexagonDrawOptions & defaults,
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

class HexWeightedMap : public HexCubeMap
{
private:
	weightedmap weights_storage;
	HexaGraph graph;
public:
	HexWeightedMap();
	HexWeightedMap(const QList<ICube> & coordlist, QMap<ICube, int> & weights, const int size, QPoint offset, 
		hexagonDrawOptions & defaults = HEX_DEFAULTS,bool pnty=true, QString & def_tag = QString());
	HexWeightedMap(const std::string & fname, const int size, QPoint & offset, 
		hexagonDrawOptions & defaults = HEX_DEFAULTS, bool pnty = true, QString & def_tag = QString());
	void setWeightForOne(const ICube & coord, const int val);
	void setWeightForTag(const QString & tag, const int val);
	void setWeightForCoordList(const QList<ICube> & coordlist, const int weight);
	void setWeightForCoordList(const std::vector<ICube> & coordlist, const int weight);
	void insertWeightsIntoMap(const weightedmap & wmap);
	int getWeight(const ICube & ic) const { if (weights_storage.contains(ic)) return weights_storage[ic]; return -999; };
	void dropWeights();
	HexaGraph & getGraph() { return graph; };
	void insert(const ICube & key, const Hexagon & value, const int weight)
	{
		if (!core_storage.contains(key))
			core_storage.insert(key, value);
		graph.insertHeight(WeightedCoord({ key, weight }));
	};
	void insert(const ICube & key, const int weight)
	{
		if (!core_storage.contains(key))
		{
			auto coords = key.toPixel(hex_size, pointy);
			core_storage.insert(key,
				Hexagon(QPoint(coords.first, coords.second), hex_size,
					coord_start.x(), coord_start.y(), HEX_DEFAULTS, pointy));
			graph.insertHeight(WeightedCoord({ key, weight }));
		}
	}
	void remove(const ICube & key);
	std::string serialize();
};

