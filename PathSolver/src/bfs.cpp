#pragma once
#include "bfs.h"

void BFSGraph::reset()
{
	Graph::reset();
	distance = {};
	frontier = {};
}

/*
Note that both functions assume that the graph has a valid start and end node. This condition is checked in the main loop before calling either of the two.
*/

//Runs A full BFS. Returns A unordered_map of parent node pairs
// Terminates early if the end node is found
void CompleteBFS(BFSGraph& bgraph) //Returns a map containing the parents of each cell
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
int StepBFS(BFSGraph& bgraph)
{
	//First check if a path exists
	//If the frontier is empty then either we haven't initialised or no path exists
	if (bgraph.frontier.empty())
	{
		//If frontier is empty and start has been explored, then no path exists
		if (bgraph[*bgraph.start_index()].isSeen()) return -1;

		//Otherwise it means the frontier hasn't been initialised yet, so we initialise.
		int start = *bgraph.start_index();
		bgraph.frontier.push(start);
		bgraph.parents[start] = -1;
		bgraph.distance[start] = 0;
		bgraph[start].setSeen();

	}


	//Assert (!bgraph.frontier.empty() ) 
	int curr = bgraph.frontier.front();
	bgraph.frontier.pop();

	bgraph[curr].Visited();

	//Done
	if (bgraph[curr].isType(Node::Type::end_)) return -1;

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