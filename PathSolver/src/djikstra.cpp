#pragma once 
#include "djikstra.h"

void DjikstraGraph::reset()
{
	Graph::reset();
	cost_so_far = {};
	//Reset priority queue
}



void CompleteDjikstra(DjikstraGraph& dgraph) //Returns a map containing the parents of each cell
{

	int start = *dgraph.start_index();

	//Create the priority queue
	auto cmp = [&dgraph](int a, int b) {return dgraph.cost_so_far[a] > dgraph.cost_so_far[b]; };
	std::priority_queue<int, std::vector<int>, decltype(cmp)> frontier(cmp);

	//Initialise
	frontier.push(start);
	dgraph.parents[start] = start;
	dgraph.cost_so_far[start] = 0;

	//Frontier contains vertices who's neighbours haven't yet been examined
	dgraph[start].setSeen();

	while (!frontier.empty())
	{
		int curr = frontier.top();
		frontier.pop();
		dgraph[curr].Visited(); //Color

		if (dgraph[curr].isType(Node::Type::end_)) return; //Early stop 

		for (int next : dgraph.get_neighbours(curr))
		{
			if (dgraph[next].isType(Node::Type::wall_)) continue; //ignore walls
			int new_cost = dgraph.cost_so_far[curr] + dgraph.cost(curr, next);
			if (!dgraph.parents.contains(next) || new_cost < dgraph.cost_so_far[next]) //Add if not seen before or we found a cheaper path
			{
				dgraph.cost_so_far[next] = new_cost; //Update the cost 
				dgraph.parents[next] = curr;
				frontier.push(next);
				dgraph[next].setSeen(); //Each node should only be processed once
			}
		}

	}
	return;
}


//On each iteration, the frontier will be printed
//Returns the index of the node that is next in the queue after the current one
//int StepDjikstra(DjikstraGraph& dgraph)
//{
//	//First check if a path exists
//	//If the frontier is empty then either we haven't initialised or no path exists
//	if (bgraph.frontier.empty())
//	{
//		//If frontier is empty and start has been explored, then no path exists
//		if (bgraph[*bgraph.start_index()].isSeen()) return -1;
//
//		//Otherwise it means the frontier hasn't been initialised yet, so we initialise.
//		int start = *bgraph.start_index();
//		bgraph.frontier.push(start);
//		bgraph.parents[start] = -1;
//		bgraph.distance[start] = 0;
//		bgraph[start].setSeen();
//
//	}
//
//
//	//Assert (!bgraph.frontier.empty() ) 
//	int curr = bgraph.frontier.front();
//	bgraph.frontier.pop();
//
//	bgraph[curr].Visited();
//
//	//Done
//	if (bgraph[curr].isType(Node::Type::end_)) return -1;
//
//	//Assert: frontier contains ...
//
//	for (int i : bgraph.get_neighbours(curr))
//	{
//		if (!bgraph[i].isSeen() && !bgraph[i].isType(Node::Type::wall_)) //Each node should only be in the queue at most once. Don't add walls to the frontier
//		{
//			bgraph.parents[i] = curr;
//			bgraph.distance[i] = bgraph.distance[curr] + 1;
//			bgraph.frontier.push(i);
//			bgraph[i].setSeen();
//		}
//	}
//
//	return bgraph.frontier.front(); //Return index of next node to be processed
//}