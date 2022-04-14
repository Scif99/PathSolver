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
	if ((index + 1) % dim != 0) ans.push_back(index + 1);//Right
	if (index + dim < dim * dim - 1) ans.push_back(index + dim); //Down
	return ans;

}


int get_start_index(const std::vector<Cell>& grid)
{
	for (int i =0;i<grid.size();++i)
	{
		if (grid[i].isStart()) return i;
	}
}

bool has_start(const std::vector<Cell>& grid)
{
	for (int i = 0;i < grid.size();++i)
	{
		if (grid[i].isStart()) return true;
	}
	return false;
}

int get_end_index(const std::vector<Cell>& grid)
{
	for (int i = 0;i < grid.size();++i)
	{
		if (grid[i].isEnd()) return i;
	}
}

bool has_end(const std::vector<Cell>& grid)
{
	for (int i = 0;i < grid.size();++i)
	{
		if (grid[i].isEnd()) return true;
	}
	return false;
}

std::unordered_map<int, int> bfs(std::vector<Cell>& grid, int dim) //Returns a map containing the parents of each cell
{
	std::unordered_map<int, int> parents;  
	std::unordered_map<int, int> distance; 
	std::queue<int> frontier; //Queue containts the indices of the cells being explored
	

	int start = get_start_index(grid);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="grid"></param>
	/// <param name="dim"></param>
	/// <returns></returns>
	frontier.push(start);
	parents[start] = -1;
	distance[start] = 0;
	grid[start].isExplored();
	//Assert: 

	while (!frontier.empty())
	{
		
		int curr = frontier.front();
		grid[curr].setExplored();
		frontier.pop();
		
		std::cerr << "Exploring: " << curr << '\n';

		if (grid[curr].isEnd()) //End early if we find the target
		{
			std::cout << "Target found in " << distance[curr]<< " steps" << '\n';
			return parents; //Done
		}

		std::vector<int> neighbours = get_neighbours(curr, dim);
		for (const auto& i : neighbours)
		{
			
			if (!grid[i].isExplored() && !grid[i].isWall()) //Don't add walls to the frontier
			{

				frontier.push(i);
				distance[i]= distance[curr]+ 1;
				grid[i].setFrontier();
				parents[i]= curr;
				
			}
		}

	}
	return parents;

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


