#pragma once

#include <queue>
#include <unordered_map>
#include<thread>
#include<chrono>

#include "Graph.h"
#include "utility.h"

std::vector<int> get_neighbours(int index, int dim)
{
	std::vector<int> ans;
	ans.reserve(4);
	if (index % dim != 0) ans.push_back(index - 1); //Left
	if (index - dim > 0) ans.push_back(index - dim); //Up
	if ((index + 1) % dim != 0) ans.push_back(index + 1);//Right
	if (index + dim < dim * dim - 1) ans.push_back(index + dim); //Down
	return ans;

}



std::unordered_map<int, int> bfs(Graph& graph) //Returns a map containing the parents of each cell
{
	std::unordered_map<int, int> parents; //data[i].first = parent. data[i].second = distance 
	std::unordered_map<int, int> distance; //data[i].first = parent. data[i].second = distance 

	std::queue<int> frontier; //Queue containts the indices of the cells being explored
	

	int start = graph.get_start();

	
	frontier.push(start);
	parents[start] = -1;
	distance[start] = 0;
	graph[start].isExplored();
	//Assert: 

	int explored = 1;

	while (!frontier.empty())
	{
		
		int curr = frontier.front();
		graph[curr].setExplored();
		frontier.pop();
		
		//std::cerr << "Exploring Node " << curr << "\tNodes Explored: " <<explored<<"\n";
		//++explored;
		//if (explored > 399) return parents;

		if (graph[curr].isEnd()) //End early if we find the target
		{
			std::cout << "Target found in " << distance[curr]<< " steps" << '\n';
			return parents; //Done
		}

		std::vector<int> neighbours = get_neighbours(curr, 20); //Dim should be a variable?
		for (const auto& i : neighbours)
		{
			
			if (!graph[i].isExplored() && !graph[i].isWall()) //Don't add walls to the frontier
			{

				frontier.push(i);
				distance[i] = distance[curr] + 1;
				graph[i].setFrontier();
				parents[i] = curr;

			}
		}

	}
	return parents;

}

void draw_path(std::unordered_map<int, int>& parents, Graph& graph)
{
	int curr = graph.get_end();
	graph[curr].setEnd(); //Re-color the end
	int dist = 0;
	while (parents[curr]>=0)
	{
		int p = parents[curr];
		graph[p].setPath();
		curr = p;

	}
	graph[graph.get_start()].setStart(); //Re-color the star
	return;
}


