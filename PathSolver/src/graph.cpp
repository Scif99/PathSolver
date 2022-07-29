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
			v_nodes_.emplace_back(j * units, i * units, w_size , sz);
		}
	}
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








