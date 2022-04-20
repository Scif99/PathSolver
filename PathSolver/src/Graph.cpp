//#pragma once
//#include "Graph.h"
//
////Fill the graph up with nodes
//void Graph::fill(float w_size)
//{
//	//Fill the grid
//	for (int i = 0; i < dim_; ++i) //Row 
//	{
//		for (int j = 0; j < dim_; ++j) //Column
//		{
//			float units = w_size / dim_;
//			v_nodes_.push_back(Node{ j * units, i * units, w_size , dim_ });//The Nodes are stored in row-major order ----->
//		}
//
//	}
//}
//
////Return a vector containing indices of a node's neighbours
//std::vector<int> Graph::get_neighbours(int index)
//{
//	std::vector<int> ans;
//	if (index % dim_ != 0) ans.push_back(index - 1); //Left
//	if (index - dim_ > 0) ans.push_back(index - dim_); //Up
//	if ((index + 1) % dim_ != 0) ans.push_back(index + 1);//Right
//	if (index + dim_ < dim_ * dim_ - 1) ans.push_back(index + dim_); //Down
//	return ans;
//
//}
//
////Set the node with given index as the end node
////Handles reseting the old start
//void Graph::addStart(int i)
//{
//	if (start_ >= 0) v_nodes_[start_].reset(); //Reset the previous start node
//	v_nodes_[i].setType(Node::Type::start_);
//	start_ = i;
//}
//
////Returns an optional start index
//std::optional<int> Graph::start_index() const
//{
//	if (start_ > -1) return start_;
//	else return std::nullopt;
//}
//
////Set the node with given index as the end node
////Handles reseting the old end
//void Graph::addEnd(int i)
//{
//	if (end_ >= 0) v_nodes_[end_].reset(); //Reset the previous start node
//	v_nodes_[i].setType(Node::Type::end_);
//	end_ = i;
//}
//
////Returns an optional end index
//std::optional<int> Graph::end_index() const
//{
//	if (end_ > -1) return end_;
//	else return std::nullopt;
//}
//
////Add a wall to the node with given index
//void Graph::addWall(int i)
//{
//	v_nodes_[i].setType(Node::Type::wall_);
//}
//
//void Graph::addGrass(int i)
//{
//	v_nodes_[i].setType(Node::Type::grass_);
//}
//
////Reset graph to blank state
//void Graph::reset()
//{
//	for (int i = 0; i < size(); ++i)
//	{
//		v_nodes_[i].reset();
//	}
//	parents = {};
//}