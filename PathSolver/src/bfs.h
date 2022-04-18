#pragma once

#include <queue>
#include <unordered_map>
#include<thread>
#include<chrono>

#include "Graph.h"
#include "utility.h"


void bfs_full(Graph& graph) //Returns a map containing the parents of each cell
{

	int start = graph.start_index();

	//Initialise
	graph.frontier.push(start);
	graph.parents[start] = -1;
	graph.distance[start] = 0;


	//Assumes a valid start exists
	graph[start].setSeen();

	while (!graph.frontier.empty())
	{

		int curr = graph.frontier.front();
		graph.frontier.pop();
		graph[curr].setVisited();

		if (graph[curr].isType(Node::Type::end_)) return; //Early stop 
	

		for (int i : graph.get_neighbours(curr))
		{
			if (!graph[i].isSeen() && !graph[i].isType(Node::Type::wall_)) //Don't add walls to the frontier
			{
				graph.parents[i] = curr;
				graph.distance[i] = graph.distance[curr] + 1;
				graph.frontier.push(i);
				graph[i].setSeen(); //Each node should only be processed once
			}
		}

	}
	return;
}


//On each iteration, the frontier will be printed
//Returns the index of the node that is next in the queue after the current one
int bfs_step(Graph& graph)
{
	//If the frontier is empty then we know a path cannot exist, but this case explicitely handled by the draw_path 
	if (graph.frontier.empty())
	{
		//If frontier is empty and start has been explored, then no path must exist
		if (graph[graph.start_index()].isSeen()) return -1;

		//Otherwise it means the frontier hasn't been initialised yet, so we initialise.
		int start = graph.start_index();
		graph.frontier.push(start);
		graph.parents[start] = -1;
		graph.distance[start] = 0;
		graph[start].setSeen();

	}

	int curr = graph.frontier.front(); //Note that the frontier will always be non
	graph.frontier.pop();
	std::cout << "Exploring " << curr << '\n';

	graph[curr].setVisited();

	if (graph[curr].isType(Node::Type::end_))
	{
		return -1; //Return this end index
	}
	//Assert: frontier contains ...

	for (int i : graph.get_neighbours(curr))
	{
		if (!graph[i].isSeen() && !graph[i].isType(Node::Type::wall_)) //Each node should only be in the queue at most once. Don't add walls to the frontier
		{
			graph.parents[i] = curr;
			graph.distance[i] = graph.distance[curr] + 1;
			graph.frontier.push(i);
			graph[i].setSeen(); 

		}
	}

	return graph.frontier.front(); //Return index of next node to be processed
}


void draw_path(Graph& graph)
{
	//Assumes the graph has a start and end
	int start = graph.start_index();
	int end = graph.end_index();

	//Check if a path exists
	auto has_end = [&](std::pair<int, int> p) {return p.first == end; }; //Lambda to check if the end node is contained
	if (std::find_if(graph.parents.begin(), graph.parents.end(), has_end) == graph.parents.end())
	{
		std::cout << "No path exists\n";
		return;
	}

	int curr = end;
	int dist = 0;

	while (graph.parents[curr] >= 0)
	{
		int p = graph.parents[curr];
		graph[p].setType(Node::Type::path_);
		curr = p;
		++dist;
	}
	std::cout << "Path found with manhatten distance = " << dist << '\n';
	graph[start].setType(Node::Type::start_); //Re-color
	graph[end].setType(Node::Type::end_); //Re-color
}

