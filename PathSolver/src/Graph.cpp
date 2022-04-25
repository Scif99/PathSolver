#pragma once
#include "Graph.h"
#include "node.h"

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