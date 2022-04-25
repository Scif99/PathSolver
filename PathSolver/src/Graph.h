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

	int cost(int in, int out) const { return v_nodes_[out].isType(Node::Type::grass_) ? 5 : 1; }
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

void DrawPath(Graph& graph);






