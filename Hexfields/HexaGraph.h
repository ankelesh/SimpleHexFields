#pragma once
#include <qset.h>
#include <qmap.h>
#include <CubeCoords.h>
#include <qvector.h>
#include <sstream>
#include <queue>
#include <memory>
#include <limits>

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
struct UniformPathfindResult
{
	bool success;
	QVector<ICube> path;
	QMap<ICube, ICube> came_from;
	QMap<ICube, int> cost_so_far;
};
typedef std::unique_ptr<UniformPathfindResult> UniPFResult;
static std::string showResults(UniPFResult & res)
{
	std::ostringstream sout;
	sout << ((res->success) ? "Success" : "Fail") <<
		" Path contains " << res->path.count();
	return sout.str();
}
static bool operator<(const WeightedCoord & f, const WeightedCoord & s) { return f.weight < s.weight; };
static bool operator>(const WeightedCoord & f, const WeightedCoord & s) { return f.weight > s.weight; };
static std::string serialize(WeightedCoord & wcoord)
{
		std::ostringstream sout;
		sout << wcoord.coord.toStr() << wcoord.weight << '\n';
		return sout.str();
}
static WeightedCoord deserialize(std::string & str)
	{
		std::istringstream sin(str);
		int x, z, y, w;
		sin >> x >> z >> y >> w;
		return WeightedCoord({ ICube(x,z,y), w });
	}
class HexaGraph
{
private:
	QMap<ICube, int> * WeightedCoords;
	QSet<ICube> heights;
	QMap<ICube, QSet<ICube>> edges;
public:
	HexaGraph(QMap<ICube, int> * wghtmap);
	std::string getDescription() const;
	bool insertHeight(WeightedCoord & wc);
	bool removeHeight(const ICube & key);
	QSet<ICube> neighbours(const ICube &) const;
	QVector<ICube> BreadthFirstSearch(const ICube & start, const ICube & finish) const;
	QVector<ICube> DijkstraSearch(const ICube & start, const ICube & finish, const int max_cost=0);
	UniPFResult UniformBreadthFirst(const ICube & start, const ICube & finish) const;
	UniPFResult UniformDijkstra(const ICube & start, const ICube & finish,
		const int max_cost =std::numeric_limits<int>().max()) const;
	UniPFResult UniformBestFirst(const ICube & start, const ICube & finish) const;
	UniPFResult UniformAStar(const ICube & start, const ICube & finish,
		const int max_cost = std::numeric_limits<int>().max());
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
