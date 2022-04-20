#pragma once

#include <queue>
#include <unordered_map>
#include<thread>
#include<chrono>

#include "Graph.h"
#include "utility.h"


void bfs_full(BFSGraph& bgraph) //Returns a map containing the parents of each cell
{

	int start = *bgraph.start_index();

	//Initialise
	bgraph.frontier.push(start);
	bgraph.parents[start] = start;
	bgraph.distance[start] = 0;


	//Assumes a valid start exists
	bgraph[start].setSeen();

	while (!bgraph.frontier.empty())
	{

		int curr = bgraph.frontier.front();
		bgraph.frontier.pop();
		bgraph[curr].Visited();

		if (bgraph[curr].isType(Node::Type::end_)) return; //Early stop 
	

		for (int i : bgraph.get_neighbours(curr))
		{
			if (!bgraph[i].isSeen() && !bgraph[i].isType(Node::Type::wall_)) //Don't add walls to the frontier
			{
				bgraph.parents[i] = curr;
				bgraph.distance[i] = bgraph.distance[curr] + 1;
				bgraph.frontier.push(i);
				bgraph[i].setSeen(); //Each node should only be processed once
			}
		}

	}
	return;
}

//On each iteration, the frontier will be printed
//Returns the index of the node that is next in the queue after the current one
int bfs_step(BFSGraph& bgraph)
{
	//If the frontier is empty then we know a path cannot exist, but this case explicitly handled by the draw_path 
	if (bgraph.frontier.empty())
	{
		//If frontier is empty and start has been explored, then no path must exist
		if (bgraph[*bgraph.start_index()].isSeen()) return -1;

		//Otherwise it means the frontier hasn't been initialised yet, so we initialise.
		int start = *bgraph.start_index();
		bgraph.frontier.push(start);
		bgraph.parents[start] = -1;
		bgraph.distance[start] = 0;
		bgraph[start].setSeen();

	}

	int curr = bgraph.frontier.front(); //Note that the frontier will always be non
	bgraph.frontier.pop();
	std::cout << "Exploring " << curr << '\n';

	bgraph[curr].Visited();

	if (bgraph[curr].isType(Node::Type::end_))
	{
		return -1; //Return this end index
	}
	//Assert: frontier contains ...

	for (int i : bgraph.get_neighbours(curr))
	{
		if (!bgraph[i].isSeen() && !bgraph[i].isType(Node::Type::wall_)) //Each node should only be in the queue at most once. Don't add walls to the frontier
		{
			bgraph.parents[i] = curr;
			bgraph.distance[i] = bgraph.distance[curr] + 1;
			bgraph.frontier.push(i);
			bgraph[i].setSeen();
		}
	}

	return bgraph.frontier.front(); //Return index of next node to be processed
}



