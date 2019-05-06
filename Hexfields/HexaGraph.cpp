#include "stdafx.h"
#include "HexaGraph.h"


HexaGraph::HexaGraph(QMap<ICube, int> * wghtmap) : WeightedCoords(wghtmap)
{
	auto coord = wghtmap->begin();
	while (coord != wghtmap->end())
	{
		QSet<ICube> neighb;
		//debug << "\Getting neighbors for " << coord.key().toStr();
		for (int i = 0; i < 6; ++i)
		{
			ICube neigCoord = coord.key().neighbor((HexCoords::CubeDirections)i);
			//debug <<"\n\t"<< neigCoord.toStr();
			if (wghtmap->contains(neigCoord))
			{
				neighb.insert(neigCoord);
			}
		}
		heights.insert(coord.key());
		edges.insert(coord.key(), neighb);
		++coord;
	}
}
std::string HexaGraph::getDescription() const
{
	std::ostringstream sout;
	sout << "\nGraph contains " << heights.count() << " heights";
	auto hght = heights.begin();
	while (hght != heights.end())
	{
		sout << "\nHeight " << hght->toStr();
		auto edgeset = edges[*hght].begin();
		auto edgesetend = edges[*hght].end();
		while (edgeset != edgesetend)
		{
			sout << "\n\tHas edge: (" << hght->toStr() << ") -> ("
				<< edgeset->toStr() << ") and weight: " << (*WeightedCoords)[*hght];
			++edgeset;
		}
		++hght;
	}
	sout << "\n<Graph end>\n";
	return sout.str();
}
QSet<ICube> HexaGraph::neighbours(const ICube & coord) const
{
	if (heights.contains(coord))
	{
		return edges[coord];
	}
	return QSet<ICube>();
}
QVector<ICube> HexaGraph::BreadthFirstSearch(const ICube & start, const ICube & finish) const
{
	if (!(heights.contains(start) && heights.contains(finish)))
	{
		return QVector<ICube>();
	}
	QQueue<ICube> frontier;
	frontier.enqueue(start);
	QMap<ICube, ICube> came_from;
	came_from.insert(start, ICube(-300,150,150));
	ICube current;
	while (!frontier.isEmpty())
	{
		current = frontier.dequeue();
		if (current == finish)
			break;
		for each(ICube next in neighbours(current))
		{
			if (!came_from.contains(next))
			{
				frontier.enqueue(next);
				came_from.insert(next, current);
			}
		}
	}
	current = finish;
	QVector<ICube> path;
	while (!(current == start))
	{
		path.push_back(current);
		current = came_from[current];
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());
	return path;
}
QVector<ICube> HexaGraph::DijkstraSearch(const ICube & start, const ICube & finish, const int max_cost)
{
	std::priority_queue<WeightedCoord, std::vector<WeightedCoord>, std::greater<WeightedCoord>> frontier;
	frontier.push(WeightedCoord({ start, 0 }));
	QMap<ICube, ICube> came_from;
	QMap<ICube, int> cost_so_far;
	came_from.insert(start, ICube(-150, -150, 300));
	cost_so_far.insert(start, 0);
	WeightedCoord current;
	//debug << "Dijkstra started. Stats: S " << start.toStr() << " F " << finish.toStr();
	while (!frontier.empty())
	{
		current = frontier.top();
		frontier.pop();
		//debug << "\nCurrent changed: " << current.coord.toStr() << " w " << current.weight;
		if (current.coord == finish)
			break;
		for each(ICube next in neighbours(current.coord))
		{
			//debug << "\nNext changed: " << next.toStr();
			int new_cost = cost_so_far[current.coord] + (*WeightedCoords)[current.coord];
			//debug << " new cost " << new_cost;
			if ((!cost_so_far.contains(next)) || (new_cost < cost_so_far[next]))
			{
				cost_so_far.insert(next, new_cost);
				//debug << "\ninserted to csf: " << next.toStr() << new_cost;
				int priority = new_cost;
				frontier.push(WeightedCoord({ next, priority }));
				came_from.insert(next, current.coord);
				//debug << "\ninserted to came_from: " << next.toStr(), current.coord.toStr();
			}
		}
	}
	ICube curr = finish;
	//debug << "\nFin : " << finish.toStr() << " St: " << start.toStr() << " Curr: " << curr.toStr();
	QVector<ICube> path;
	while (!(curr == start))
	{
		path.push_back(curr);
		//debug << curr.toStr();
		curr = came_from[curr];
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());
	return path;
}
bool HexaGraph::insertHeight(WeightedCoord & wc)
{
	if (!WeightedCoords->contains(wc.coord))
	{
		WeightedCoords->insert(wc.coord, wc.weight);
		heights.insert(wc.coord);
		QSet<ICube> temp;
		for (int i = 0; i < 6; ++i)
		{
			ICube neigCoord = wc.coord.neighbor((HexCoords::CubeDirections)i);
			if (WeightedCoords->contains(neigCoord))
			{
				temp.insert(neigCoord);
				edges[neigCoord].insert(wc.coord);
			}
		}
		edges.insert(wc.coord, temp);
		return true;
	}
	return false;
}
bool HexaGraph::removeHeight(const ICube & key)
{
	if (WeightedCoords->contains(key))
	{
		WeightedCoords->remove(key);
		auto neighbours = edges[key];
		auto begin = neighbours.begin();
		while (begin != neighbours.end())
		{
			edges[*begin].remove(key);
			++begin;
		}
		heights.remove(key);
		edges.remove(key);
		return true;
	}
	return false;
}
std::unique_ptr<UniformPathfindResult> HexaGraph::UniformBreadthFirst(const ICube & start, const ICube & finish) const
{
	std::unique_ptr<UniformPathfindResult> uniresult(new UniformPathfindResult);
	uniresult->success = true;
	if (!(heights.contains(start) && heights.contains(finish)))
	{
		uniresult->success = false;
		return uniresult;
	}
	QQueue<ICube> frontier;
	frontier.enqueue(start);
	uniresult->came_from.insert(start, ICube(-300, 150, 150));
	ICube current;
	while (!frontier.isEmpty())
	{
		current = frontier.dequeue();
		if (current == finish)
			break;
		for each(ICube next in neighbours(current))
		{
			if (!uniresult->came_from.contains(next))
			{
				frontier.enqueue(next);
				uniresult->came_from.insert(next, current);
			}
		}
	}
	current = finish;
	while (!(current == start))
	{
		uniresult->path.push_back(current);
		if (uniresult->came_from.contains(current))
			current = uniresult->came_from[current];
		else
		{
			uniresult->success = false;
			return uniresult;
		}
	}
	uniresult->path.push_back(start);
	std::reverse(uniresult->path.begin(), uniresult->path.end());
	return uniresult;
}
std::unique_ptr<UniformPathfindResult>  HexaGraph::UniformDijkstra(const ICube & start, const ICube & finish,
	const int max_cost) const
{
	std::unique_ptr<UniformPathfindResult> uniresult(new UniformPathfindResult);
	uniresult->success=true;
	if (!(heights.contains(start) && heights.contains(finish)))
	{
		uniresult->success = false;
		return uniresult;
	}
	std::priority_queue<WeightedCoord, std::vector<WeightedCoord>,
		std::greater<WeightedCoord>> frontier;
	frontier.push(WeightedCoord({ start, 0 }));
	uniresult->came_from.insert(start, ICube(-150, -150, 300));
	uniresult->cost_so_far.insert(start, 0);
	WeightedCoord current;
	//debug << "Dijkstra started. Stats: S " << start.toStr() << " F " << finish.toStr();
	while (!frontier.empty())
	{
		current = frontier.top();
		frontier.pop();
		//debug << "\nCurrent changed: " << current.coord.toStr() << " w " << current.weight;
		if (current.coord == finish)
			break;
		for each(ICube next in neighbours(current.coord))
		{
			//debug << "\nNext changed: " << next.toStr();
			int new_cost = uniresult->cost_so_far[current.coord] +
				(*WeightedCoords)[next];
			//debug << " new cost " << new_cost;
			if (((!uniresult->cost_so_far.contains(next)) || 
				(new_cost < uniresult->cost_so_far[next])) && new_cost < max_cost)
			{
				uniresult->cost_so_far.insert(next, new_cost);
				//debug << "\ninserted to csf: " << next.toStr() << new_cost;
				int priority = new_cost;
				frontier.push(WeightedCoord({ next, priority }));
				uniresult->came_from.insert(next, current.coord);
				//debug << "\ninserted to came_from: " << next.toStr(), current.coord.toStr();
			}
		}
	}
	ICube curr = finish;
	//debug << "\nFin : " << finish.toStr() << " St: " << start.toStr() << " Curr: " << curr.toStr();
	while (!(curr == start))
	{
		uniresult->path.push_back(curr);
		//debug << curr.toStr();
		if (uniresult->came_from.contains(curr))
			curr = uniresult->came_from[curr];
		else
		{
			uniresult->success = false;
			return uniresult;
		}
	}
	uniresult->path.push_back(start);
	std::reverse(uniresult->path.begin(), uniresult->path.end());
	return uniresult;
}
UniPFResult HexaGraph::UniformBestFirst(const ICube & start, const ICube & finish) const
{
	std::priority_queue<WeightedCoord, std::vector<WeightedCoord>,
		std::greater<WeightedCoord>> frontier;
	frontier.push(WeightedCoord({ start, 0 }));
	UniPFResult uniresult(new UniformPathfindResult);
	uniresult->success = true;
	if (!(heights.contains(start) && heights.contains(finish)))
	{
		uniresult->success = false;
		return uniresult;
	}
	uniresult->came_from[start] = ICube(-300, 150, 150);
	while (!frontier.empty())
	{
		WeightedCoord current = frontier.top();
		frontier.pop();
		if (current.coord == finish)
		{
			break;
		}
		for each(ICube next in neighbours(current.coord))
		{
			if (!(uniresult->came_from.contains(next)))
			{
				int priority = finish.distanceTo(next);
				frontier.push(WeightedCoord({ next, priority }));
				uniresult->came_from.insert(next, current.coord);
			}
		}
	}
	auto current = finish;
	while (!(current == start))
	{
		uniresult->path.push_back(current);
		if (uniresult->came_from.contains(current))
			current = uniresult->came_from[current];
		else
		{
			uniresult->success = false;
			return uniresult;
		}
	}
	uniresult->path.push_back(start);
	std::reverse(uniresult->path.begin(), uniresult->path.end());
	return uniresult;
}
UniPFResult HexaGraph::UniformAStar(const ICube & start, const ICube & finish,
	const int max_cost)
{
	std::unique_ptr<UniformPathfindResult> uniresult(new UniformPathfindResult);
	uniresult->success = true;
	if (!(heights.contains(start) && heights.contains(finish)))
	{
		uniresult->success = false;
		return uniresult;
	}
	std::priority_queue<WeightedCoord, std::vector<WeightedCoord>,
		std::greater<WeightedCoord>> frontier;
	frontier.push(WeightedCoord({ start, 0 }));
	uniresult->came_from.insert(start, ICube(-150, -150, 300));
	uniresult->cost_so_far.insert(start, 0);
	WeightedCoord current;
	//debug << "Dijkstra started. Stats: S " << start.toStr() << " F " << finish.toStr();
	while (!frontier.empty())
	{
		current = frontier.top();
		frontier.pop();
		//debug << "\nCurrent changed: " << current.coord.toStr() << " w " << current.weight;
		if (current.coord == finish)
			break;
		for each(ICube next in neighbours(current.coord))
		{
			//debug << "\nNext changed: " << next.toStr();
			int new_cost = uniresult->cost_so_far[current.coord] +
				(*WeightedCoords)[next];
			//debug << " new cost " << new_cost;
			if (((!uniresult->cost_so_far.contains(next)) ||
				(new_cost < uniresult->cost_so_far[next])) && new_cost < max_cost)
			{
				uniresult->cost_so_far.insert(next, new_cost);
				//debug << "\ninserted to csf: " << next.toStr() << new_cost;
				int priority = new_cost + finish.distanceTo(next)*3;
				frontier.push(WeightedCoord({ next, priority }));
				uniresult->came_from.insert(next, current.coord);
				//debug << "\ninserted to came_from: " << next.toStr(), current.coord.toStr();
			}
		}
	}
	ICube curr = finish;
	//debug << "\nFin : " << finish.toStr() << " St: " << start.toStr() << " Curr: " << curr.toStr();
	while (!(curr == start))
	{
		uniresult->path.push_back(curr);
		//debug << curr.toStr();
		if (uniresult->came_from.contains(curr))
			curr = uniresult->came_from[curr];
		else
		{
			uniresult->success = false;
			return uniresult;
		}
	}
	uniresult->path.push_back(start);
	std::reverse(uniresult->path.begin(), uniresult->path.end());
	return uniresult;
}
