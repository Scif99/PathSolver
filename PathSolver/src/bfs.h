#pragma once

#include <queue>
#include <unordered_map>
#include<thread>
#include<chrono>

#include "Graph.h"
#include "utility.h"


//std::unordered_map<int, int> bfs(Graph& graph) //Returns a map containing the parents of each cell
//{
//	std::unordered_map<int, int> parents;
//	std::unordered_map<int, int> distance; 
//
//	int start = graph.get_start();
//	parents[start] = -1;
//	distance[start] = 0;
//
//	std::queue<int> frontier; //Queue containts the indices of the cells being explored
//	frontier.push(start);
//
//
//	graph[start].setSeen();
//	//Assert: frontier contains ...
//
//	while (!frontier.empty())
//	{
//		int curr = frontier.front();
//		frontier.pop();
//
//		if (graph[curr].isEnd())
//		{
//			color_frontier(frontier,graph); //Color elements that were in the frontier separately
//			return parents; //Early stop 
//		}
//
//		for (int i : graph.get_neighbours(curr))
//		{
//			if (!graph[i].isSeen() && !graph[i].isWall()) //Don't add walls to the frontier
//			{
//				parents[i] = curr;
//				distance[i] = distance[curr] + 1;
//				frontier.push(i);
//				graph[i].setSeen(); //Each node should ol
//			}
//		}
//
//	}
//	return parents;
//
//}

void draw_path(Graph& graph)
{

	int start = graph.get_start();
	int end = graph.get_end();

	
	//Check if a path exists
	auto has_end = [&](std::pair<int, int> p) {return p.first == end; }; //Lambda to check if the end node is contained
	if (std::find_if(graph.parents.begin(), graph.parents.end(), has_end) == graph.parents.end())
	{
		std::cout << "No path exists\n";
		return;
	}

	int curr = end;
	int dist = 0;

	while (graph.parents[curr]>=0)
	{
		int p = graph.parents[curr];
		graph[p].setPath();
		curr = p;
		++dist;
	}
	std::cout << "Path found with manhatten distance = " << dist << '\n';
	graph[start].setStart(); //Re-color
	graph[end].setEnd(); //Re-color
}

//On each iteration, the frontier will be printed
//Returns the index of the node that is next in the queue after the current one

int bfs_step(Graph& graph)
{



	if (graph.frontier.empty()) return -1; //If the frontier is empty then we know a path cannot exist, but this case explicitely handled by the draw_path function


	int curr = graph.frontier.front(); //Note that the frontier will always be non
	graph.frontier.pop();


	if (graph[curr].isStart()) graph[curr].setSeen();	
	graph[curr].setVisited();

	if (graph[curr].isEnd())
	{
		return -1; //Return this end index
	}
	//Assert: frontier contains ...

	for (int i : graph.get_neighbours(curr))
	{
		if (!graph[i].isSeen() && !graph[i].isWall()) //Each node should only be in the queue at most once. Don't add walls to the frontier
		{
			graph.parents[i] = curr;
			graph.distance[i] = graph.distance[curr] + 1;
			graph.frontier.push(i);
			graph[i].setSeen(); 

		}
	}

	return graph.frontier.front(); //Return index of the node that is now at the fron of the queue
}


