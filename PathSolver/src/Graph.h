#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include "node.h"
#include <iterator>
#include <optional>
#include <cstdlib>
#include <iostream>


class Graph 
{
public:
	Graph(int dim)
		:dim_{ dim }, v_nodes_{ {} }, start_{std::nullopt}, end_{ std::nullopt }
	{}

	int dim() const { return dim_; } //Dimensions of the graph.
	int size() const { return v_nodes_.size(); } //Total number of Nodes in the graph.
	void fill(float w_size); //Fill an empty graph with nodes
	std::vector<int> get_neighbours(int index);

	//Accessors
	const Node& operator[](int i) const  { return v_nodes_[i]; }
	Node& operator[](int i) { return v_nodes_[i]; }

	std::optional<int> start_index() const; 
	void addStart(int i);

	std::optional<int> end_index() const;
	void addEnd(int i);

	void addWall(int i);
	void addGrass(int i);
	virtual void reset();

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

//Returns an optional start index
std::optional<int> Graph::start_index() const
{
	if (start_) return start_.value();
	else return std::nullopt;
}

//Set the node with given index as the end node, resetting the old one if necessary
void Graph::addEnd(int i)
{
	if (end_) v_nodes_[end_.value()].reset(); //Reset the previous start node
	v_nodes_[i].setType(Node::Type::end_);
	end_ = i;
}

//Returns an optional end index
std::optional<int> Graph::end_index() const
{
	if (end_) return end_.value();
	else return std::nullopt;
}

//Add a wall to the node with given index
//Walls have no weight, they cannot be explored
void Graph::addWall(int i)
{
	v_nodes_[i].setType(Node::Type::wall_);
}

//Grass is a node with weight 5
void Graph::addGrass(int i)
{
	v_nodes_[i].setType(Node::Type::grass_);
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

/*
Different specialisations of Graph, each for a different algorithm.

*/

struct BFSGraph : Graph
{
	BFSGraph(int dim): Graph(dim) {}
	int distance_to(int i) { return distance[i]; }
	void reset() override;
	std::unordered_map<int, int> distance; // For non-weighted
	std::queue<int> frontier; //BFS uses a queue
};

void BFSGraph::reset()
{
	Graph::reset();
	distance = {};
	frontier = {};
}

struct DjikstraGraph : Graph
{
	DjikstraGraph(int dim): Graph(dim) {}
	int cost(int in, int out) const { return Graph::operator[](out).isType(Node::Type::grass_) ? 5 : 1; }
	int distance_to(int i) { return cost_so_far[i]; }
	void reset() override;
	std::unordered_map<int, int> cost_so_far;

	//std::priority_queue<int, std::vector<int>, decltype([&](int a, int b) {return cost_so_far[a] >cost_so_far[b]; }> frontier;
};

void DjikstraGraph::reset()
{
	Graph::reset();
	cost_so_far = {};
 //Reset priority queue
}

struct GreedyBFS : Graph
{
	GreedyBFS(int dim) : Graph(dim) {}
	int cost(int in, int out) const { return Graph::operator[](out).isType(Node::Type::grass_) ? 5 : 1; }
	//int distance_to(int i) { return cost_so_far[i]; }
	int heuristic(int a, int b) const { return std::abs((a % dim()) - (b % dim())) + (std::abs(a - b) / dim()); } //xdiff + ydiff
	void reset() override;
	std::unordered_map<int, int> priority;
	//std::priorityqueue
};

void GreedyBFS::reset()
{
	Graph::reset();
	priority = {};
	//Reset priority queue
}


struct AstarGraph : Graph 
{
	AstarGraph(int dim) : Graph(dim) {}
	int cost(int in, int out) const { return Graph::operator[](out).isType(Node::Type::grass_) ? 5 : 1; }
	int distance_to(int i) { return cost_so_far[i]; }
	int heuristic(int a, int b) const { return std::abs((a % dim()) - (b % dim())) + (std::abs(a - b) / dim());} //xdiff + ydiff
	void reset() override;
	std::unordered_map<int, int> cost_so_far;
	std::unordered_map<int, int> priority;
	//std::priorityqueue
};

void AstarGraph::reset()
{
	Graph::reset();
	cost_so_far = {};
	priority = {};
	//Reset priority queue
}



void draw_path(Graph& graph)
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