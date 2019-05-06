#include "stdafx.h"
#include "Demo.h"


Demo::Demo(QWidget * parent)
	:QWidget(parent), mainspace(new QTabWidget(this)), lay(new QHBoxLayout(this))
	, hredactor(new HexRedactor(std::string("weightsmap.txt"), QSize(500,500), 
		lineopts,this))
{
	preset_demos.reserve(6);
	redactable_demos.resize(A_star_redactable + 1);
	this->setLayout(lay);
	lay->addWidget(mainspace);
	preset_demos.push_back(new BreadthDemo(
		std::string("breadth.txt"), 50, 500, lineopts,this->size(), this));
	preset_demos.push_back(new FirstMatchDemo(
		std::string("firstmatch.txt"), 50, 500, lineopts, size(), this));
	preset_demos.push_back(new DijkstraDemo(
		std::string("dijkstra.txt"), 50, 500, lineopts, size(), this));
	preset_demos.push_back(new LimitedDijkstraDemo(
		std::string("weightsmap.txt"), 50, 500, lineopts, size(), this));
	preset_demos.push_back(new AStarDemo(
		std::string("weightsmap.txt"), 50, 500, lineopts, size(), this));
	preset_demos.push_back(new LimitedAStarDemo(
		std::string("weightsmap.txt"), 50, 500, lineopts, size(), this));
	redactable_demos[Dijkstra_redactable] = new DijkstraRedactableDemo
	(std::string("weightsmap.txt"), 50, 500, lineopts, size(), this);
	redactable_demos[A_star_redactable] = new AStarRedactableDemo(
		std::string("std.txt"), 50, 500, lineopts, size(), this);
	mainspace->addTab(preset_demos[Breadth], "Breadth First search");
	mainspace->addTab(preset_demos[First_match], "First-match search");
	mainspace->addTab(preset_demos[Dijkstra], "Dijkstra search");
	mainspace->addTab(preset_demos[Dijkstra_limited], "Dijkstra limited search");
	mainspace->addTab(preset_demos[A_star], "A* algorythm");
	mainspace->addTab(preset_demos[A_star_limited], "Limited A* algorythm");
	mainspace->addTab(redactable_demos[Dijkstra_redactable], "Dijkstra redactable");
	mainspace->addTab(redactable_demos[A_star_redactable], "A* redactable");
	mainspace->addTab(hredactor, "Hexmap redactor");
}

