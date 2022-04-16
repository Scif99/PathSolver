#pragma once

#include <unordered_map>
#include <vector>
#include "node.h"

class Graph 
{
public:
	Graph(int dim);

	int dim() const { return dim_; }
	int size() const { return v_nodes_.size(); }
	void fill(float w_size);
	std::vector<int> get_neighbours(int index);

	//Accessors
	const Node& operator[](int i) const  { return v_nodes_[i]; }
	Node& operator[](int i) { return v_nodes_[i]; }


	int get_start();
	bool has_start();
	int get_end();
	bool has_end();

	std::unordered_map<int, int> parents;
	std::unordered_map<int, int> distance;
	
private:
	int dim_;
	std::vector<Node> v_nodes_;

};


Graph::Graph(int dim)
	:dim_{ dim }, v_nodes_{ {} }
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

//Return the index of the starting node
int Graph::get_start()
{
	for (int i = 0;i < size();++i)
	{
		if (v_nodes_[i].isStart()) return i;
	}
}

//Check if the graph has a starting node
bool Graph::has_start()
{
	for (int i = 0;i < size();++i)
	{
		if (v_nodes_[i].isStart()) return true;
	}
	return false;
}

//Return the index of the end node
int Graph::get_end()
{
	for (int i = 0;i < size();++i)
	{
		if (v_nodes_[i].isEnd()) return i;
	}
}

//Check if the graph has an end node
bool Graph::has_end()
{
	for (int i = 0;i < size();++i)
	{
		if (v_nodes_[i].isEnd()) return true;
	}
	return false;
}