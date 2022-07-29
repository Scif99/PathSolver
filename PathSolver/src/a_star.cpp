#include "a_star.h"



/* A* (A-Star)
* -----------------------------------------------------
* - A combination of Greedy + Djikstra
* - Djikstra -> Greedy best first search -> A*
* - Only difference between A* and Djikstra is that A* uses a heuristic when calculating the new cost
* - In Djikstra the frontier compares cost_so_far, whereas in A* it compares cost_so_far + heuristic
* ---------------------------------------------------------
* */
void AstarGraph::run() //Returns a map containing the parents of each cell
{

	int start = start_index().value();
	int end = end_index().value();

	priority[start] = heuristic(start, end);
	PQ.push(std::make_pair(priority[start], start));
	parents[start] = start;
	cost_so_far[start] = 0;

	//Frontier contains vertices who's neighbours haven't yet been examined
	//Assumes a valid start exists
	this->operator[](start).setSeen();

	while (!PQ.empty())
	{
		int curr = PQ.top().second;
		PQ.pop();

		this->operator[](curr).Visited();

		if (this->operator[](curr).isType(Node::Type::end_)) return; //Early stop 

		for (int next : get_neighbours(curr))
		{
			if (this->operator[](next).isType(Node::Type::wall_)) continue; //ignore walls

			int new_cost = cost_so_far[curr] + cost(curr, next);
			if (!parents.contains(next) || new_cost < cost_so_far[next]) //Add if not seen before or we found a cheaper path
			{
				cost_so_far[next] = new_cost; //Update cost
				parents[next] = curr;
				priority[next] = new_cost + heuristic(next, end);
				PQ.push(std::make_pair(priority[next], next));
				this->operator[](next).setSeen(); //Each node should only be processed once
			}
		}

	}
	return;
}

int AstarGraph::step() //Returns a map containing the parents of each cell
{

	int start = start_index().value();
	int end = end_index().value();
	if (PQ.empty())
	{
		//If frontier is empty and start has been explored, then no path exists
		if (this->operator[](start_index().value()).isSeen()) return -1;

		//Otherwise it means the frontier hasn't been initialised yet, so we initialise.
		PQ.push(std::make_pair(heuristic(start, end), start));
		parents[start] = -1;
		priority[start] = heuristic(start, end);
		this->operator[](start).setSeen();

	}
	int curr = PQ.top().second;
	PQ.pop();

	this->operator[](curr).Visited();

	if (this->operator[](curr).isType(Node::Type::end_)) return -1; //Early stop 

	for (int next : get_neighbours(curr))
	{
		if (this->operator[](next).isType(Node::Type::wall_)) continue; //ignore walls

		int new_cost = cost_so_far[curr] + cost(curr, next);
		if (!parents.contains(next) || new_cost < cost_so_far[next]) //Add if not seen before or we found a cheaper path
		{
			cost_so_far[next] = new_cost; //Update cost
			parents[next] = curr;
			priority[next] = new_cost + heuristic(next, end);
			PQ.push(std::make_pair(new_cost + heuristic(next, end), next));
			this->operator[](next).setSeen(); //Each node should only be processed once
		}
	}
	return PQ.top().second;
}

void AstarGraph::reset()
{
	Graph::reset();
	cost_so_far = {};
	priority = {};
	PQ = {};
}