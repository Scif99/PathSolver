#pragma once
#include "Graph.h"

struct AstarGraph : Graph
{
	AstarGraph(int dim) : Graph(dim) {}
	int distance_to(int i) { return cost_so_far[i]; }
	int heuristic(int a, int b) const { return std::abs((a % dim()) - (b % dim())) + (std::abs(a - b) / dim()); } //xdiff + ydiff
	virtual void reset() override;
	void run() override;
	int step() override;
	std::unordered_map<int, int> cost_so_far;
	std::unordered_map<int, int> priority;
	std::priority_queue < std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater <std::pair<int, int>>> PQ; //(cost_so_far, index)
};