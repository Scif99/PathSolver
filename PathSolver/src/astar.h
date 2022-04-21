#pragma once

#include "Graph.h"

/* A* (A-star)
* - Very similar to 
* - Djikstra -> Greedy best first search -> A*
* - Only difference between A* and Djikstra is that A* uses a heuristic when calculating the new cost
* - It is the best of both worlds between Djikstra and Greedy best first search
*/

void AstarFull(AstarGraph& agraph) //Returns a map containing the parents of each cell
{

	int start = *agraph.start_index();

	//Create the priority queue
	auto cmp = [&agraph](int a, int b) {return agraph.cost_so_far[a] > agraph.cost_so_far[b]; };
	std::priority_queue<int, std::vector<int>, decltype(cmp)> frontier(cmp);

	//Initialise
	//dgraph.pq.push(start);
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


		for (int i : agraph.get_neighbours(curr))
		{
			if (agraph[i].isType(Node::Type::wall_)) continue; //ignore walls

			int new_cost = agraph.cost_so_far[curr] + agraph.cost(curr, i);
			if (!agraph.parents.contains(i) || new_cost < agraph.cost_so_far[i]) //Add if not seen before or we found a cheaper path
			{
				agraph.cost_so_far[i] = new_cost + agraph.heuristic(curr, i);
				agraph.parents[i] = curr;
				frontier.push(i);
				agraph[i].setSeen(); //Each node should only be processed once
			}
		}

	}
	return;
}
