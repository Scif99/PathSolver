#pragma once
#include "Graph.h"


/* --------------------------------
	Djikstra
	--------------------------------
*/
struct DjikstraGraph : Graph
{
	DjikstraGraph(int dim) : Graph(dim) {}
	int distance_to(int i) { return cost_so_far[i]; }
	virtual void reset() override;
	void run()override;
	int step() override;
	std::unordered_map<int, int> cost_so_far;
	std::priority_queue < std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater <std::pair<int, int>>> PQ; //(cost_so_far, index)
};