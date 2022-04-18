#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include "node.h"

class Graph 
{
public:
	Graph(int dim);

	int dim() const { return dim_; } //Dimensions of the graph.
	int size() const { return v_nodes_.size(); } //Total number of Nodes in the graph.
	void fill(float w_size); //Fill an empty graph with nodes
	std::vector<int> get_neighbours(int index);

	//Accessors
	const Node& operator[](int i) const  { return v_nodes_[i]; }
	Node& operator[](int i) { return v_nodes_[i]; }


	int start_index() const { return start_; }
	void addStart(int i);

	int end_index() const { return end_; }
	void addEnd(int i);

	void addWall(int i);
	void reset();



	//Iterators
	using iterator = std::vector<Node>::iterator;
	using const_iterator = std::vector<Node>::const_iterator;

	iterator begin() { return v_nodes_.begin(); }
	iterator end() { return v_nodes_.begin(); }
	const_iterator cbegin() const { return v_nodes_.cbegin(); }
	const_iterator cend() const { return v_nodes_.cend(); }



	//Data to store information about a search
	std::unordered_map<int, int> distance;
	std::queue<int> frontier;
	std::unordered_map<int, int> parents;
	
private:
	int dim_;
	std::vector<Node> v_nodes_;
	int start_;
	int end_;
};


Graph::Graph(int dim)
	:dim_{ dim }, v_nodes_{ {} }, start_{ -1 }, end_{ -1 }
{}

//Fill the graph up with nodes
void Graph::fill(float w_size) 
{
	//Fill the grid
	for (int i = 0; i < dim_; ++i) //Row 
	{
		for (int j = 0;j < dim_;++j) //Column
		{
			float units = w_size / dim_;
			v_nodes_.push_back(Node{ j * units, i * units, w_size , dim_});//The Nodes are stored in row-major order ----->
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

//Set the node with given index as the end node
//Handles reseting the old end
void Graph::addEnd(int i)
{
	if (end_ >= 0) v_nodes_[end_].reset(); //Reset the previous start node
	v_nodes_[i].setType(Node::Type::end_);
	end_ = i;
}

//Add a wall to the node with given index
void Graph::addWall(int i)
{
	v_nodes_[i].setType(Node::Type::wall_);
}

//Reset graph to blank state
void Graph::reset()
{
	for (int i = 0; i < size(); ++i)
	{
		v_nodes_[i].reset();
	}

	//Clear data from previous search
	frontier = {};
	parents = {};
	distance = {};
}