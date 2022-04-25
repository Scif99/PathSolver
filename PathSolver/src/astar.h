#pragma once

#include "Graph.h"
#include "node.h"


/* A* (A-star)
* - Very similar to 
* - Djikstra -> Greedy best first search -> A*
* - Only difference between A* and Djikstra is that A* uses a heuristic when calculating the new cost
* - In Djikstra the frontier compares cost_so_far, whereas in A* it compares cost_so_far + heuristic
- It is the best of both worlds between Djikstra and Greedy best first search
*/

struct AstarGraph : Graph
{
	AstarGraph(int dim) : Graph(dim) {}
	int distance_to(int i) { return cost_so_far[i]; }
	int heuristic(int a, int b) const { return std::abs((a % dim()) - (b % dim())) + (std::abs(a - b) / dim()); } //xdiff + ydiff
	void reset() override;
	std::unordered_map<int, int> cost_so_far;
	std::unordered_map<int, int> priority;
	//std::priorityqueue
};

void AstarFull(AstarGraph& agraph);