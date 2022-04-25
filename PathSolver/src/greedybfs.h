#pragma once

#include "Graph.h"

struct GreedyBFS : Graph
{
	GreedyBFS(int dim) : Graph(dim) {}
	//int distance_to(int i) { return cost_so_far[i]; }
	int heuristic(int a, int b) const { return std::abs((a % dim()) - (b % dim())) + (std::abs(a - b) / dim()); } //xdiff + ydiff
	void reset() override;
	std::unordered_map<int, int> priority;
	//std::priorityqueue
};

void GreedyFull(GreedyBFS& ggraph);