#pragma once

#include <queue>

#include "cell.h"
#include "utility.h"

std::vector<int> get_neighbours(int index, int dim)
{

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

void bfs(std::vector<Cell>& grid)
{
	int start = get_start_index(grid);	
	grid[start].setPath(); 

	std::queue<int> frontier; //Queue containts the indices of the cells being explored
	frontier.push(start);
	//Assert: 

	while (!frontier.empty())
	{
		int curr = frontier.front();
		frontier.pop();

		if (grid[curr].isEnd())
		{
			std::cout << curr;
			return; //Done
		}

		std::vector<int> neighbours = get_neighbours(curr, 20); //DIM VARIABLE?
		for (int i : neighbours) std::cout << i << '\n';
		for (int i : neighbours)
		{
			if (i < 0 || i >= grid.size()) continue; //Bounds check
			if (!grid[i].isPath())
			{
				grid[i].setPath();
				frontier.push(i);
			}
		}

	}
	return;

}


