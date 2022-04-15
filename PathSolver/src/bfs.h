#pragma once

#include <queue>
#include <unordered_map>
#include<thread>
#include<chrono>

#include "Graph.h"
#include "utility.h"



std::unordered_map<int, int> bfs(Graph& graph) //Returns a map containing the parents of each cell
{
	std::unordered_map<int, int> parents;
	std::unordered_map<int, int> distance; 

	int start = graph.get_start();
	parents[start] = -1;
	distance[start] = 0;

	std::queue<int> frontier; //Queue containts the indices of the cells being explored
	frontier.push(start);


	graph[start].setSeen();
	//Assert: frontier contains ...

	while (!frontier.empty())
	{
		int curr = frontier.front();
		frontier.pop();

		if (graph[curr].isEnd()) return parents; //Early stop 

		for (int i : graph.get_neighbours(curr))
		{
			if (!graph[i].isSeen() && !graph[i].isWall()) //Don't add walls to the frontier
			{
				parents[i] = curr;
				distance[i] = distance[curr] + 1;
				frontier.push(i);
				//graph[i].setFrontier();
				graph[i].setSeen(); //Each node should ol
			}
		}

	}
	return parents;

}

void draw_path(std::unordered_map<int, int>& parents, Graph& graph)
{
	int start = graph.get_start();
	int end = graph.get_end();
	
	//Check if a path exists
	auto has_end = [&](std::pair<int, int> p) {return p.first == end; }; //Lambda to check if the end node is contained
	if (std::find_if(parents.begin(), parents.end(), has_end) == parents.end())
	{
		std::cout << "No path exists\n";
		return;
	}

	int curr = end;
	int dist = 0;

	while (parents[curr]>=0)
	{
		int p = parents[curr];
		graph[p].setPath();
		curr = p;
		++dist;
	}
	std::cout << "Path found with distance = " << dist << '\n';
	graph[end].setEnd(); //Re-color the end
	return;
}


