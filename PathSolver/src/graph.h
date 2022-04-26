#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include <iterator>
#include <optional>
#include <cstdlib>
#include <iostream>

#include "node.h"


/*
* The Graph clase is an abstract base class 
* Any class which inherits must implement a run() and step () function.s

*/


class Graph 
{
public:
	Graph(int dim)
		:dim_{ dim }, v_nodes_{ {} }, start_{std::nullopt}, end_{ std::nullopt }
	{}
	virtual ~Graph() = default; //Destructor must be marked virtual
	void fill(int w_size); //Fill an empty graph with nodes

	int dim() const { return dim_; } //Dimensions of the graph.
	int size() const { return v_nodes_.size(); } //Total number of nodes in the graph.
	std::vector<int> get_neighbours(int index); //Returns a vector containing indices of a nodes neighbours

	//Accessors
	const Node& operator[](int i) const  { return v_nodes_[i]; }
	Node& operator[](int i) { return v_nodes_[i]; }

	//Start getter/setter
	std::optional<int> start_index() const { return start_ ? start_ : std::optional<int>{}; };
	void addStart(int i);

	//End getter/setter
	std::optional<int> end_index() const { return end_ ? end_ : std::optional<int>{}; };
	void addEnd(int i); 

	void addWall(int i) { v_nodes_[i].setType(Node::Type::wall_); }; //Place a wall at the node with given index. Walls cannot be passed through.
	void addGrass(int i) { v_nodes_[i].setType(Node::Type::grass_); }; //Place grass at the node with given index. Grass has a weight of 5

	int cost(int in, int out) const { return v_nodes_[out].isType(Node::Type::grass_) ? 5 : 1; } //Returns the cost of an edge
	virtual void reset(); //Reset graph to a blank one

	//Ideally want this to be pure virtual...
	virtual int step() { return -1; };
	virtual void run() {};

	void drawPath();

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


/* -----------------
	BFS
	---------------------
*/
struct BFSGraph : Graph
{
	BFSGraph(int dim) : Graph(dim) {}
	int distance_to(int i) { return distance[i]; }
	virtual void reset() override;
	void run() override;
	int step() override;
	std::unordered_map<int, int> distance; // For non-weighted
	std::queue<int> frontier; //BFS uses a queue to process nodes
};

/* --------------------------------
	Djikstra
	--------------------------------
*/
struct DjikstraGraph : Graph
{
	DjikstraGraph(int dim) : Graph(dim) {}
	int distance_to(int i) { return cost_so_far[i]; }
	virtual void reset() override;
	void run()override;
	int step() override;
	std::unordered_map<int, int> cost_so_far;
	std::priority_queue < std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater <std::pair<int, int>>> PQ; //(cost_so_far, index)
};

/* ----------------------
*	Greedy
	--------------------------
*/
struct GreedyGraph : Graph
{
	GreedyGraph(int dim) : Graph(dim) {}
	//int distance_to(int i) { return cost_so_far[i]; }
	int heuristic(int a, int b) const { return std::abs((a % dim()) - (b % dim())) + (std::abs(a - b) / dim()); } //xdiff + ydiff
	virtual void reset() override;
	void run() override;
	int step() override;
	std::unordered_map<int, int> priority;
	std::priority_queue < std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater <std::pair<int, int>>> PQ; //(cost_so_far, index)
};

/* -----------------------
	A*
   ------------------------
*/
struct AstarGraph : Graph
{
	AstarGraph(int dim) : Graph(dim) {}
	int distance_to(int i) { return cost_so_far[i]; }
	int heuristic(int a, int b) const { return std::abs((a % dim()) - (b % dim())) + (std::abs(a - b) / dim()); } //xdiff + ydiff
	virtual void reset() override;
	void run() override;
	int step() override;
	std::unordered_map<int, int> cost_so_far;
	std::unordered_map<int, int> priority;
	std::priority_queue < std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater <std::pair<int, int>>> PQ; //(cost_so_far, index)
};

