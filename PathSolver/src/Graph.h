#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include "node.h"
#include <iterator>
#include <optional>
#include <cstdlib>


class Graph 
{
public:
	Graph(int dim)
		:dim_{ dim }, v_nodes_{ {} }, start_{  }, end_{ -1 }
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
	//std::unordered_map<int, int> distance; // For non-weighted
	//std::queue<int> frontier;
	std::unordered_map<int, int> parents;
	//std::unordered_map<int, int> cost_so_far;
	
private:
	int dim_;
	std::vector<Node> v_nodes_;
	int start_;
	int end_;
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

//Set the node with given index as the end node
//Handles reseting the old start
void Graph::addStart(int i)
{
	if (start_ >= 0) v_nodes_[start_].reset(); //Reset the previous start node
	v_nodes_[i].setType(Node::Type::start_);
	start_ = i;
}

//Returns an optional start index
std::optional<int> Graph::start_index() const
{
	if (start_ > -1) return start_;
	else return std::nullopt;
}

//Set the node with given index as the end node
//Handles reseting the old end
void Graph::addEnd(int i)
{
	if (end_ >= 0) v_nodes_[end_].reset(); //Reset the previous start node
	v_nodes_[i].setType(Node::Type::end_);
	end_ = i;
}

//Returns an optional end index
std::optional<int> Graph::end_index() const
{
	if (end_ > -1) return end_;
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
	for (int i = 0; i < size(); ++i)
	{
		v_nodes_[i].reset();
	}
	parents = {};
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
	std::unordered_map<int, int> cost_so_far;
	//std::priority_queue
};


struct Astar : Graph 
{
	Astar(int dim) : Graph(dim) {}
	int cost(int in, int out) const { return Graph::operator[](out).isType(Node::Type::grass_) ? 5 : 1; }
	int distance_to(int i) { return cost_so_far[i]; }
	//double heuristic(int a, int b) const {return std::abs() }
	std::unordered_map<int, int> cost_so_far;




};


template<typename Graph>
void draw_path(Graph& graph)
{
	//Assumes the graph has a start and end
	int start = *graph.start_index();
	int end = *graph.end_index();

	//Check if a path exists
	auto has_end = [&](std::pair<int, int> p) {return p.first == end; }; //Lambda to check if the end node is contained
	if (std::find_if(graph.parents.begin(), graph.parents.end(), has_end) == graph.parents.end())
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
	std::cout << "Path found with manhatten distance = " << graph.distance_to(end) << '\n';
	graph[start].setType(Node::Type::start_); //Re-color
	graph[end].setType(Node::Type::end_); //Re-color
}