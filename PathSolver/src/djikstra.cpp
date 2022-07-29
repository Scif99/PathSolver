#include "djikstra.h"

/*------------------------------
* DJIKSTRA
*  Generalisation of BFS to Weighted graph
* - Instead of measuring 'no. of steps from source' we measure the 'weighted distance from source'
* - Instead of a queue we use a (min) priority queue, ordered by total weight from source
* - It is a Greedy algorithm - at each iteration we pick the node with the shortest distance
*
* (Aside)
* - This algorithm is technically 'Uniform Cost Search' and not 'Djikstra'
* - Djikstra will find the shortest path to *any* node, while UCS is for single-source paths
- In Djikstra, all distances are initialised as infinity and added to the priority queue at the start.
- In UCS, the priority queue is filled 'lazily', which can save space.
	-------------------------------
*/
void DjikstraGraph::run() //Returns a map containing the parents of each cell
{

	int start = start_index().value();

	//Initialise
	PQ.push(std::make_pair(0, start));
	parents[start] = start;
	cost_so_far[start] = 0;

	//Frontier contains vertices who's neighbours haven't yet been examined
	this->operator[](start).setSeen();

	while (!PQ.empty())
	{
		int curr = PQ.top().second; //Get index of node to process
		PQ.pop();
		this->operator[](curr).Visited(); //Color

		if (this->operator[](curr).isType(Node::Type::end_)) return; //Early stop 

		for (int next : get_neighbours(curr))
		{
			if (this->operator[](next).isType(Node::Type::wall_)) continue; //ignore walls
			int new_cost = cost_so_far[curr] + cost(curr, next);
			if (!parents.contains(next) || new_cost < cost_so_far[next]) //Add if not seen before or we found a cheaper path
			{
				cost_so_far[next] = new_cost; //Update the cost 
				parents[next] = curr;
				PQ.push(std::make_pair(new_cost, next));
				this->operator[](next).setSeen(); //Each node should only be processed once
			}
		}
	}
	return;
}


//Runs one step of Djikstra, returning the index of the next node to process
int DjikstraGraph::step()
{
	//First check if a path exists
	//If the frontier is empty then either we haven't initialised or no path exists
	if (PQ.empty())
	{
		//If frontier is empty and start has been explored, then no path exists
		if (this->operator[](start_index().value()).isSeen()) return -1;

		//Otherwise it means the frontier hasn't been initialised yet, so we initialise.
		int start = start_index().value();
		PQ.push(std::make_pair(0, start));
		parents[start] = -1;
		cost_so_far[start] = 0;
		this->operator[](start).setSeen();

	}

	//Assert (!PQ.empty() ) 
	int curr = PQ.top().second;
	PQ.pop();

	this->operator[](curr).Visited();

	//Done
	if (this->operator[](curr).isType(Node::Type::end_)) return -1;

	//Assert: frontier contains ...

	for (int next : get_neighbours(curr))
	{
		if (this->operator[](next).isType(Node::Type::wall_)) continue; //ignore walls
		int new_cost = cost_so_far[curr] + cost(curr, next);
		if (!parents.contains(next) || new_cost < cost_so_far[next]) //Each node should only be in the queue at most once. Don't add walls to the frontier
		{
			parents[next] = curr;
			cost_so_far[next] = new_cost;
			PQ.push(std::make_pair(new_cost, next));
			this->operator[](next).setSeen();
		}
	}
	return PQ.top().second; //Return index of next node to be processed
}

//Reset graph to clean slate
void DjikstraGraph::reset()
{
	Graph::reset();
	cost_so_far = {};
	PQ = {};
}
