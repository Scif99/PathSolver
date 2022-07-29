#include "bfs.h"

/* BFS
* ---------------------------------
*  Finds the shortest path for unweighted graphs
*  Uses a Queue to process nodes. Nodes are not prioritied in any way.
*  Ensures that each node is processed no more than once.
	-------------------------------
*/

//Runs A full BFS. Returns A unordered_map of {parent:node} pairs.
// Terminates early if the end node is found
void BFSGraph::run()
{
	int start = start_index().value();

	//Initialise
	frontier.push(start);
	parents[start] = start;
	distance[start] = 0;


	//Assumes a valid start exists
	this->operator[](start).setSeen();

	while (!frontier.empty())
	{
		int curr = frontier.front();
		frontier.pop();
		this->operator[](curr).Visited();

		if (this->operator[](curr).isType(Node::Type::end_)) return; //Early stop 

		for (int i : get_neighbours(curr))
		{
			if (!this->operator[](i).isSeen() && !this->operator[](i).isType(Node::Type::wall_)) //Don't add walls to the frontier
			{
				parents[i] = curr;
				distance[i] = distance[curr] + 1;
				frontier.push(i);
				this->operator[](i).setSeen(); //Each node should only be processed once
			}
		}
	}
	return;
}


//Runs one step of BFS, returning the index of the next node to process
int BFSGraph::step()
{
	//First check if a path exists
	//If the frontier is empty then either we haven't initialised or no path exists
	if (frontier.empty())
	{
		//If frontier is empty and start has been explored, then no path exists
		if (this->operator[](start_index().value()).isSeen()) return -1;

		//Otherwise it means the frontier hasn't been initialised yet, so we initialise.
		int start = start_index().value();
		frontier.push(start);
		parents[start] = -1;
		distance[start] = 0;
		this->operator[](start).setSeen();

	}


	//Assert (!bgraph.frontier.empty() ) 
	int curr = frontier.front();
	frontier.pop();

	this->operator[](curr).Visited();

	//Done
	if (this->operator[](curr).isType(Node::Type::end_)) return -1;

	//Assert: frontier contains ...

	for (int i : get_neighbours(curr))
	{
		if (!this->operator[](i).isSeen() && !this->operator[](i).isType(Node::Type::wall_)) //Each node should only be in the queue at most once. Don't add walls to the frontier
		{
			parents[i] = curr;
			distance[i] = distance[curr] + 1;
			frontier.push(i);
			this->operator[](i).setSeen();
		}
	}

	return frontier.front(); //Return index of next node to be processed
}

//Reset BFSGraph to blank state.
void BFSGraph::reset()
{
	Graph::reset();
	distance = {};
	frontier = {};
}