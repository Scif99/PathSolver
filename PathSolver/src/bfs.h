#pragma once

#include <queue>
#include <unordered_map>
#include<thread>
#include<chrono>

#include "cell.h"
#include "utility.h"

std::vector<int> get_neighbours(int index, int dim)
{
	std::vector<int> ans;
	ans.reserve(4);
	if (index % dim != 0) ans.push_back(index - 1); //Left
	if (index - dim > 0) ans.push_back(index - dim); //Up
	if (index + 1 % dim != 0) ans.push_back(index + 1);//Right
	if (index + dim < dim * dim - 1) ans.push_back(index + dim); //Down
	return ans;

}

/*
Returns the index of the starting cell

Assumes that a start node exists
*/
int get_start_index(const std::vector<Cell>& grid)
{
	for (int i =0;i<grid.size();++i)
	{
		if (grid[i].isStart()) return i;
	}
}

int get_end_index(const std::vector<Cell>& grid)
{
	for (int i = 0;i < grid.size();++i)
	{
		if (grid[i].isEnd()) return i;
	}
}

std::unordered_map<int,int> bfs(std::vector<Cell>& grid) //Returns a map containing the parents of each cell
{
	std::unordered_map<int, int> parent; //parent[i] is the index from which i came from.
	std::unordered_map<int, int> distance; //distance[i] is the distance of cell at index i from the start.
	std::queue<int> frontier; //Queue containts the indices of the cells being explored
	

	int start = get_start_index(grid);	
	

	
	frontier.push(start);
	parent[start] = -1;
	distance[start] = 0;
	grid[start].isExplored();
	//Assert: 

	while (!frontier.empty())
	{
		
		int curr = frontier.front();
		grid[curr].setExplored();
		frontier.pop();
		
		//std::cerr << "Exploring: " << curr << '\n';

		if (grid[curr].isEnd()) //End early if we find the target
		{
			std::cout << "Target found in " << distance[curr] << " steps" << '\n';
			return parent; //Done
		}

		std::vector<int> neighbours = get_neighbours(curr, 20); //Dim should be a variable?
		for (int i : neighbours)
		{
			
			if (!grid[i].isExplored() && !grid[i].isWall()) //Don't add walls to the frontier
			{
				//std::cerr << i << " is an unseen neighbour of " << curr << '\n';
				frontier.push(i);
				distance[i] = distance[curr] + 1;
				grid[i].setFrontier();
				parent[i] = curr;
				
			}

			//else std::cout << i << " has been seen before... ignoring " << '\n';
		}

	}
	return parent;

}

void draw_path(std::unordered_map<int, int>& parents, std::vector<Cell>& grid)
{
	int curr = get_end_index(grid);
	grid[curr].setEnd(); //Re-color the end
	int dist = 0;
	while (parents[curr]>=0)
	{
		int p = parents[curr];
		grid[p].setPath();
		curr = p;

	}
	grid[get_start_index(grid)].setStart(); //Re-color the star
	return;
}


