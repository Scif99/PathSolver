#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include <iterator>
#include <optional>
#include <cstdlib>
#include <iostream>

#include "node.h"







class Graph 
{
public:
	Graph(int dim)
		:dim_{ dim }, v_nodes_{ {} }, start_{std::nullopt}, end_{ std::nullopt }
	{}

	int dim() const { return dim_; } //Dimensions of the graph.
	int size() const { return v_nodes_.size(); } //Total number of nodes in the graph.
	void fill(float w_size); //Fill an empty graph with nodes
	std::vector<int> get_neighbours(int index); //Returns a vector containing indices of a nodes neighbours

	//Accessors
	const Node& operator[](int i) const  { return v_nodes_[i]; }
	Node& operator[](int i) { return v_nodes_[i]; }

	//Start getter/setter
	std::optional<int> start_index() const { return start_ ? start_ : std::optional<int>{}; };
	void addStart(int i); //Sets the node with the given index to be the start

	//End getter/setter
	std::optional<int> end_index() const { return end_ ? end_ : std::optional<int>{}; };
	void addEnd(int i); 

	void addWall(int i) { v_nodes_[i].setType(Node::Type::wall_); }; //Place a wall at the node with given index. Walls cannot be passed through.
	void addGrass(int i) { v_nodes_[i].setType(Node::Type::grass_); }; //Place grass at the node with given index. Grass has a weight of 5

	int cost(int in, int out) const { return v_nodes_[out].isType(Node::Type::grass_) ? 5 : 1; } //Returns the cost of an edge
	virtual void reset(); //Reset graph to a blank one

	//Iterators
	using iterator = std::vector<Node>::iterator;
	using const_iterator = std::vector<Node>::const_iterator;

	iterator begin() { return v_nodes_.begin(); }
	iterator end() { return v_nodes_.end(); }
	const_iterator cbegin() const { return v_nodes_.cbegin(); }
	const_iterator cend() const { return v_nodes_.cend(); }

	//Data to store information about a search
	std::unordered_map<int, int> parents;

	
private:
	int dim_;
	std::vector<Node> v_nodes_;
	std::optional<int> start_; //Using optionals rather than using -1 as null
	std::optional<int> end_;
};

//Fill the graph up with nodes
void Graph::fill(float w_size)
{
	//Fill the grid
	for (int i = 0; i < dim_; ++i) //Row 
	{
		for (int j = 0; j < dim_; ++j) //Column
		{
			float units = w_size / dim_;
			v_nodes_.push_back(Node{ j * units, i * units, w_size , dim_ });//The Nodes are stored in row-major order ----->
		}
	}
}

//Return a vector containing indices of a node's neighbours
std::vector<int> Graph::get_neighbours(int index)
{
	std::vector<int> ans;
	if (index % dim_ != 0) ans.push_back(index - 1); //Left
	if (index - dim_ > 0) ans.push_back(index - dim_); //Up
	if ((index + 1) % dim_ != 0) ans.push_back(index + 1);//Right
	if (index + dim_ < dim_ * dim_ - 1) ans.push_back(index + dim_); //Down
	return ans;

}

//Set the node with given index as the start node, resetting the old one if necessary
void Graph::addStart(int i)
{
	if (start_) v_nodes_[start_.value()].reset(); //Reset the previous start node
	v_nodes_[i].setType(Node::Type::start_);
	start_ = i;
}

//Set the node with given index as the end node, resetting the old one if necessary
void Graph::addEnd(int i)
{
	if (end_) v_nodes_[end_.value()].reset(); //Reset the previous start node
	v_nodes_[i].setType(Node::Type::end_);
	end_ = i;
}

//Reset graph to blank state
void Graph::reset()
{
	int sz = v_nodes_.size();
	for (int i = 0; i < sz; ++i)
	{
		v_nodes_[i].reset();
	}
	parents = {};

	//Reset the start and end indices to null
	start_ = std::nullopt;
	end_ = std::nullopt;
}


/* -----------------
	BFS
	---------------------
*/

struct BFSGraph : Graph
{
	BFSGraph(int dim) : Graph(dim) {}
	int distance_to(int i) { return distance[i]; }
	void reset() override;
	void run();
	int step();
	std::unordered_map<int, int> distance; // For non-weighted
	std::queue<int> frontier; //BFS uses a queue to process nodes
};

void BFSGraph::reset()
{
	Graph::reset();
	distance = {};
	frontier = {};
}

//Runs A full BFS. Returns A unordered_map of parent node pairs
// Terminates early if the end node is found
void BFSGraph::run() //Returns a map containing the parents of each cell
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

//On each iteration, the frontier will be printed
//Returns the index of the node that is next in the queue after the current one
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

/* --------------------------------
	Djikstra
	--------------------------------
*/
/*
* ---------------------------------
*  Generalisation of BFS to Weighted graph
* - Instead of measuring 'no of steps from source' we measure the 'weighted distance from source'
* - Frontier is now a priority queue, ordered by total weight from source
* - It is a Greedy algorithm - at each iteration we pick the node with the shortest distance
* -
- Really in Djikstra, all distances are set to infinity initially
- This takes up a lot of space
- Techincally our algorithm is a 'uniform cost search'
	-------------------------------
*/



struct PQ
{
	using p = std::pair<int, int>;
	//std::priority_queue<p, std::vector<p>, >;


};


struct DjikstraGraph : Graph
{
	
	DjikstraGraph(int dim) : Graph(dim) {}

	int distance_to(int i) { return cost_so_far[i]; }
	void reset() override;
	void run();
	int step();
	std::unordered_map<int, int> cost_so_far;
	std::priority_queue < std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater <std::pair<int, int>>> PQ; //(cost_so_far, index)
};


void DjikstraGraph::reset()
{
	Graph::reset();
	cost_so_far = {};
	PQ = {};
}



void DjikstraGraph::run() //Returns a map containing the parents of each cell
{

	int start = start_index().value();

	//Initialise
	PQ.push(std::make_pair( 0,start));
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
				PQ.push(std::make_pair(new_cost,next));
				this->operator[](next).setSeen(); //Each node should only be processed once
			}
		}
	}
	return;
}


//On each iteration, the frontier will be printed
//Returns the index of the node that is next in the queue after the current one
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
		PQ.push(std::make_pair(0,start));
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
			PQ.push(std::make_pair(new_cost,next));
			this->operator[](next).setSeen();
		}
	}
	return PQ.top().second; //Return index of next node to be processed
}

/* ----------------------
*	Greedy
	--------------------------
*/

struct GreedyGraph : Graph
{
	GreedyGraph(int dim) : Graph(dim) {}
	//int distance_to(int i) { return cost_so_far[i]; }
	int heuristic(int a, int b) const { return std::abs((a % dim()) - (b % dim())) + (std::abs(a - b) / dim()); } //xdiff + ydiff
	void run();
	int step();
	void reset() override;
	std::unordered_map<int, int> priority;
	std::priority_queue < std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater <std::pair<int, int>>> PQ; //(cost_so_far, index)
};


void GreedyGraph::reset()
{
	Graph::reset();
	priority = {};
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
	PQ.push(std::make_pair(heuristic(start, end),start));
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
				PQ.push(std::make_pair(heuristic(next, end),next));
				this->operator[](next).setSeen(); //Each node should only be processed once
			}
		}

	}
	return;
}

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
		priority[start] = heuristic(start,end);
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

/* -----------------------
	A*
   ------------------------
*/
struct AstarGraph : Graph
{
	AstarGraph(int dim) : Graph(dim) {}
	int distance_to(int i) { return cost_so_far[i]; }
	int heuristic(int a, int b) const { return std::abs((a % dim()) - (b % dim())) + (std::abs(a - b) / dim()); } //xdiff + ydiff
	void run();
	int step();
	void reset() override;
	std::unordered_map<int, int> cost_so_far;
	std::unordered_map<int, int> priority;
	std::priority_queue < std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater <std::pair<int, int>>> PQ; //(cost_so_far, index)
};

/* A* (A-star)
* - Very similar to
* - Djikstra -> Greedy best first search -> A*
* - Only difference between A* and Djikstra is that A* uses a heuristic when calculating the new cost
* - In Djikstra the frontier compares cost_so_far, whereas in A* it compares cost_so_far + heuristic
- It is the best of both worlds between Djikstra and Greedy best first search
*/


void AstarGraph::reset()
{
	Graph::reset();
	cost_so_far = {};
	priority = {};
	PQ = {};
}

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

/*
HELPERS
*/
void DrawPath(Graph& graph)
{
	//Assumes the graph has a start and end
	int start = *graph.start_index();
	int end = *graph.end_index();

	//Check if a path exists
	if (!graph.parents.contains(end))
	{
		std::cout << "No path exists\n";
		return;
	}

	int curr = end;
	while (graph.parents[curr] != start)
	{
		int p = graph.parents[curr];
		graph[p].setType(Node::Type::path_);
		curr = p;
	}
	//std::cout << "Path found with manhatten distance = " << graph.distance_to(end) << '\n';
	graph[start].setType(Node::Type::start_); //Re-color
	graph[end].setType(Node::Type::end_); //Re-color
}