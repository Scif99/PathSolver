#pragma once

#include "Graph.h"

/* A* (A-star)
* - Very similar to 
* - Djikstra -> Greedy best first search -> A*
* - Only difference between A* and Djikstra is that A* uses a heuristic when calculating the new cost
* - In Djikstra the frontier compares cost_so_far, whereas in A* it compares cost_so_far + heuristic
- It is the best of both worlds between Djikstra and Greedy best first search
*/

void AstarFull(AstarGraph& agraph) //Returns a map containing the parents of each cell
{

	int start = *agraph.start_index();
	int end = *agraph.end_index();

	//Create the priority queue
	auto cmp = [&agraph](int a, int b) {return agraph.priority[a] > agraph.priority[b]; };
	std::priority_queue<int, std::vector<int>, decltype(cmp)> frontier(cmp);

	agraph.priority[start] = agraph.heuristic(start, end);
	frontier.push(start);
	agraph.parents[start] = start;
	agraph.cost_so_far[start] = 0;

	//Frontier contains vertices who's neighbours haven't yet been examined
	//Assumes a valid start exists
	agraph[start].setSeen();

	while (!frontier.empty())
	{
		int curr = frontier.top();
		frontier.pop();
		agraph[curr].Visited();

		if (agraph[curr].isType(Node::Type::end_)) return; //Early stop 

		for (int next : agraph.get_neighbours(curr))
		{
			if (agraph[next].isType(Node::Type::wall_)) continue; //ignore walls

			int new_cost = agraph.cost_so_far[curr] + agraph.cost(curr, next);
			if (!agraph.parents.contains(next) || new_cost < agraph.cost_so_far[next]) //Add if not seen before or we found a cheaper path
			{
				agraph.cost_so_far[next] = new_cost; //Update cost
				agraph.parents[next] = curr;
				agraph.priority[next] = new_cost + agraph.heuristic(next, end);
				frontier.push(next);
				agraph[next].setSeen(); //Each node should only be processed once
			}
		}

	}
	return;
}
