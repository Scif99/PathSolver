#include "graph.h"


//Fill the graph up with nodes
void Graph::fill(int w_size)
{
	auto sz = dim_;
	float units = w_size / sz; //How far along/down the Node's position will be
	for (int i = 0; i < sz; ++i) //Row 
	{
		for (int j = 0; j < sz; ++j) //Column
		{
			v_nodes_.push_back(Node{ j * units, i * units, w_size , sz });//The Nodes are stored in row-major order ----->
		}
	}
	std::cout << "Filled\n";
}

//Return a vector containing indices of a node's neighbours
std::vector<int> Graph::get_neighbours(int index)
{
	std::vector<int> ans;
	ans.reserve(4); //Allocate space for 4 elements.
	if (index % dim_ != 0) ans.push_back(index - 1); //Left
	if (index - dim_ > 0) ans.push_back(index - dim_); //Up
	if ((index + 1) % dim_ != 0) ans.push_back(index + 1);//Right
	if (index + dim_ < dim_ * dim_ - 1) ans.push_back(index + dim_); //Down
	return ans;
}

//Set the node with given index as the start node, resetting the old one if necessary
void Graph::addStart(int i)
{
	if (start_) v_nodes_[start_.value()].reset(); //Reset old start
	v_nodes_[i].setType(Node::Type::start_);
	start_ = i;
}

//Set the node with given index as the end node, resetting the old one if necessary
void Graph::addEnd(int i)
{
	if (end_) v_nodes_[end_.value()].reset(); //Reset old end
	v_nodes_[i].setType(Node::Type::end_);
	end_ = i;
}

//Draws the path from start to end, by following parent pointers
//Does nothing if no path exists
void Graph::drawPath()
{
	//Assumes the graph has a start and end
	int start = start_index().value();
	int end = end_index().value();

	//Check if a path exists
	if (!parents.contains(end))
	{
		std::cout << "No path exists\n";
		return;
	}

	int curr = end;
	while (parents[curr] != start)
	{
		int p = parents[curr];
		this->operator[](p).setType(Node::Type::path_);
		curr = p;
	}
	//std::cout << "Path found with manhatten distance = " << graph.distance_to(end) << '\n';
	this->operator[](start).setType(Node::Type::start_); //Re-color
	this->operator[](end).setType(Node::Type::end_); //Re-color
}

//Reset graph to blank state.
void Graph::reset()
{
	int sz = v_nodes_.size();
	for (int i = 0; i < sz; ++i)
	{
		v_nodes_[i].reset();
	}
	parents = {};

	//Reset the start and end indices
	start_ = std::nullopt;
	end_ = std::nullopt;
}


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


//In GReedy, the PQ prioritises nodes that are closer to the end


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


