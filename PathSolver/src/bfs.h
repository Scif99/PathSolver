#pragma once
#include "Graph.h"
/* -----------------
	BFS
	---------------------
*/
struct BFSGraph : Graph
{
	BFSGraph(int dim) : Graph(dim) {}
	int distance_to(int i) { return distance[i]; }
	virtual void reset() override;
	void run() override;
	int step() override;
	std::unordered_map<int, int> distance; // For non-weighted
	std::queue<int> frontier; //BFS uses a queue to process nodes
};