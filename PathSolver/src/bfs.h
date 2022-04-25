#pragma once

#include "Graph.h"

struct BFSGraph : Graph
{
	BFSGraph(int dim) : Graph(dim) {}
	int distance_to(int i) { return distance[i]; }
	void reset() override;
	std::unordered_map<int, int> distance; // For non-weighted
	std::queue<int> frontier; //BFS uses a queue to process nodes
};

void CompleteBFS(BFSGraph& bgraph);
int StepBFS(BFSGraph& bgraph);




