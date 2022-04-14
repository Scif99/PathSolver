#pragma once
#pragma once

#include <unordered_map>
#include <vector>

#include "node.h"

class Graph 
{
public:
	Graph(int dim);
	int dim() const { return dim_; }
	int size() const { return (dim_ * dim_) - 1; }
	void fill(int w_size);

	const Node& operator[](int i) const  { return v_nodes_[i]; }
	Node& operator[](int i) { return v_nodes_[i]; }


	int get_start_index();
	bool has_start();
	int get_end_index();
	bool has_end();

private:
	int dim_;
	std::vector<Node> v_nodes_;
	//std::unordered_map<int, int> parent_;
	//std::unordered_map<int, int> distance_;
};

Graph::Graph(int dim)
	:dim_{ dim }, v_nodes_{ {} }
{}

void Graph::fill(int w_size) //Fill the graph with nodes
{
	//Fill the grid
	for (int i = 0; i < dim_; ++i) //Row 
	{
		for (int j = 0;j < dim_;++j) //Column
		{
			int units = w_size / dim_;
			v_nodes_.push_back(Node{ j * units, i * units, w_size });//The Nodes are stored in row-major order ----->
		}

	}
}
int Graph::get_start_index()
{
	for (int i = 0;i < dim_;++i)
	{
		if (v_nodes_[i].isStart()) return i;
	}
}

bool Graph::has_start()
{
	for (int i = 0;i < dim_;++i)
	{
		if (v_nodes_[i].isStart()) return true;
	}
	return false;
}

int Graph::get_end_index()
{
	for (int i = 0;i < dim_;++i)
	{
		if (v_nodes_[i].isEnd()) return i;
	}
}

bool Graph::has_end()
{
	for (int i = 0;i < dim_;++i)
	{
		if (v_nodes_[i].isEnd()) return true;
	}
	return false;
}