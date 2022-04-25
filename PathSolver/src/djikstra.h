#pragma once

#include "Graph.h"


/*Djikstra
* - Generalisation of BFS to Weighted graph
* - Instead of measuring 'no of steps from source' we measure the 'weighted distance from source' 
* - Frontier is now a priority queue, ordered by total weight from source
* - It is a Greedy algorithm - at each iteration we pick the node with the shortest distance
* - 

*/

/*
- Really in Djikstra, all distances are set to infinity initially
- This takes up a lot of space
- Techincally our algorithm is a 'uniform cost search'
*/

struct DjikstraGraph : Graph
{
	DjikstraGraph(int dim) : Graph(dim) {}

	int distance_to(int i) { return cost_so_far[i]; }
	void reset() override;
	std::unordered_map<int, int> cost_so_far;
	//std::priority_queue<int, std::vector<int>, decltype([this](int a, int b) {return cost_so_far[a] > this->cost_so_far[b]; })> frontier;
};

int StepDjikstra(DjikstraGraph& dgraph);

