#include "greedy.h"

//In Greedy, the PQ prioritises nodes that are closer to the end


void GreedyGraph::run() //Returns a map containing the parents of each cell
{
	int start = start_index().value();
	int end = end_index().value();

	//Create the priority queue
	//Nodes with lowest priority will be at the front

	priority[start] = heuristic(start, end);
	PQ.push(std::make_pair(heuristic(start, end), start));
	parents[start] = start;


	this->operator[](start).setSeen();

	while (!PQ.empty())
	{
		int curr = PQ.top().second; //Get index of node to process
		PQ.pop();
		this->operator[](curr).Visited(); //This is just a visual change

		if (this->operator[](curr).isType(Node::Type::end_)) return; //Early stop 

		for (int next : get_neighbours(curr))
		{
			if (!parents.contains(next) && !this->operator[](next).isType(Node::Type::wall_))
			{
				parents[next] = curr;
				priority[next] = heuristic(next, end);
				PQ.push(std::make_pair(heuristic(next, end), next));
				this->operator[](next).setSeen(); //Each node should only be processed once
			}
		}

	}
	return;
}

//Runs one step of Djikstra, returning the index of the next node to process
int GreedyGraph::step()
{
	//First check if a path exists
	//If the frontier is empty then either we haven't initialised or no path exists

	int start = start_index().value();
	int end = end_index().value();
	if (PQ.empty())
	{
		//If frontier is empty and start has been explored, then no path exists
		if (this->operator[](start_index().value()).isSeen()) return -1;

		//Otherwise it means the frontier hasn't been initialised yet, so we initialise.
		PQ.push(std::make_pair(0, start));
		parents[start] = -1;
		priority[start] = heuristic(start, end);
		this->operator[](start).setSeen();

	}
	//Assert (!PQ.empty()) 
	int curr = PQ.top().second;
	PQ.pop();

	this->operator[](curr).Visited();

	//Done
	if (this->operator[](curr).isType(Node::Type::end_)) return -1;

	for (int next : get_neighbours(curr))
	{
		if (!parents.contains(next) && !this->operator[](next).isType(Node::Type::wall_))
		{
			parents[next] = curr;
			priority[next] = heuristic(next, end);
			PQ.push(std::make_pair(heuristic(next, end), next));
			this->operator[](next).setSeen(); //Each node should only be processed once
		}
	}
	return PQ.top().second; //Return index of next node to be processed
}

void GreedyGraph::reset()
{
	Graph::reset();
	priority = {};
	PQ = {};
}