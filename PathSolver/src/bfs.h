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
	return std::vector<int>{ index - 1,index + 1,index - dim,index + dim }; //left, right, up, down
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

std::unordered_map<int,int> bfs(std::vector<Cell>& grid)
{
	std::unordered_map<int, int> parent; //parent[i] is the index from which i came from.
	int start = get_start_index(grid);	
	grid[start].setPath(); 

	std::queue<int> frontier; //Queue containts the indices of the cells being explored
	frontier.push(start);
	parent[start] = -1;
	//Assert: 

	while (!frontier.empty())
	{
		
		int curr = frontier.front();
		frontier.pop();

		if (grid[curr].isEnd())
		{

			return parent; //Done
		}

		std::vector<int> neighbours = get_neighbours(curr, 20); //DIM VARIABLE?
		for (int i : neighbours)
		{
			if (i < 0 || i >= grid.size()) continue; //Bounds check
			if (!grid[i].isPath())
			{
				grid[i].setPath();
				frontier.push(i);
				parent[i] = curr;
			}
		}

	}
	return parent;

}

void draw_path(std::unordered_map<int, int>& parents, std::vector<Cell>& grid)
{
	int curr = get_end_index(grid);
	while (parents[curr] >= 0)
	{
		std::cout << curr << '\n';
		int p = parents[curr];
		grid[p].setWall();
		curr = p;

	}
	grid[get_start_index(grid)].setWall();
	return;
}


