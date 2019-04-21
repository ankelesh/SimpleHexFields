#pragma once
#include <qset.h>
#include <qmap.h>
#include <CubeCoords.h>
#include <qvector.h>
#include <sstream>
#include <queue>

using namespace HexCoords;
static uint qHash(const ICube & key)
{
	return key.hash();
}

struct WeightedCoord
{
	ICube coord;
	int weight;
};
static bool operator<(const WeightedCoord & f, const WeightedCoord & s) { return f.weight < s.weight; };
static bool operator>(const WeightedCoord & f, const WeightedCoord & s) { return f.weight > s.weight; };
class HexaGraph
{
private:
	QMap<ICube, int> * WeightedCoords;
	QSet<ICube> heights;
	QMap<ICube, QSet<ICube>> edges;
public:
	HexaGraph(QMap<ICube, int> * wghtmap);
	std::string getDescription() const;
	QSet<ICube> neighbours(const ICube &) const;
	QVector<ICube> BreadthFirstSearch(const ICube & start, const ICube & finish) const;
	QVector<ICube> DijkstraSearch(const ICube & start, const ICube & finish, const int max_cost=0);
};


std::string static testGraph()
{
	QMap<ICube, int> coordlist;
	coordlist.insert(ICube(0, 0, 0), 1);
	coordlist.insert(ICube(-1, 0, 1), 1);
	coordlist.insert(ICube(1, 0, -1), 1);
	coordlist.insert(ICube(0, 1, -1), 1);
	coordlist.insert(ICube(0, -1, 1), 1);
	coordlist.insert(ICube(1, -1, 0), 1);
	coordlist.insert(ICube(-1, 1, 0), 1);
	HexaGraph hg(&coordlist);
	return hg.getDescription();
}
std::string static testGraph(QList<ICube> & clist)
{
	auto coord = clist.begin();
	QMap<ICube, int> temp;
	while (coord != clist.end())
	{
		temp.insert(*coord, 1);
		++coord;
	}
	HexaGraph hg(&temp);
	return hg.getDescription();
}
std::string static testBreadth(QList<ICube> & clist, ICube start, ICube finish)
{
	auto coord = clist.begin();
	QMap<ICube, int> temp;
	while (coord != clist.end())
	{
		temp.insert(*coord, 1);
		++coord;
	}
	HexaGraph hg(&temp);
	std::ostringstream sout;
	sout << "\nPath from " << start.toStr() << " to " << finish.toStr() << '\n';
	for each(ICube coor in hg.BreadthFirstSearch(start, finish))
	{
		sout << coor.toStr() << "\n";
	}
	return sout.str();
}
static std::string testDijkstra(QList<ICube> & clist, ICube start, ICube finish)
{
	auto coord = clist.begin();
	QMap<ICube, int> temp;
	while (coord != clist.end())
	{
		temp.insert(*coord, 1);
		++coord;
	}
	HexaGraph hg(&temp);
	std::ostringstream sout;
	sout << "\nPath from " << start.toStr() << " to " << finish.toStr() << '\n';
	for each(ICube coor in hg.DijkstraSearch(start, finish, 3))
	{
		sout << coor.toStr() << "\n";
	}
	return sout.str();
}
