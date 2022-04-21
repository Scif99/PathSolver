#pragma once

#include "Graph.h"




void GreedyFull(GreedyBFS& ggraph) //Returns a map containing the parents of each cell
{

	int start = *ggraph.start_index();
	int end = *ggraph.end_index();


	//Create the priority queue
	//Nodes with lowest priority will be at the front
	auto cmp = [&ggraph](int a, int b) {return ggraph.priority[a] > ggraph.priority[b]; };
	std::priority_queue<int, std::vector<int>, decltype(cmp)> frontier(cmp);

	ggraph.priority[start] = ggraph.heuristic(start, end);
	frontier.push(start);
	ggraph.parents[start] = start;


	//ggraph[start].setSeen();

	while (!frontier.empty())
	{
		int curr = frontier.top();
		frontier.pop();
		std::cout<<"Exploring "<<curr<<'\n';
		std::cout << "Priority of " << curr << " is " << ggraph.priority[curr] << '\n';
		ggraph[curr].Visited(); //This is just a visual change

		if (ggraph[curr].isType(Node::Type::end_)) return; //Early stop 

		for (int next : ggraph.get_neighbours(curr))
		{
			if (!ggraph.parents.contains(next) && !ggraph[next].isType(Node::Type::wall_))
			{
				ggraph.parents[next] = curr;
				ggraph.priority[next] = ggraph.heuristic(next, end);
				frontier.push(next);
				ggraph[next].setSeen(); //Each node should only be processed once
			}
		}

	}
	return;
}