#pragma once

#include <queue>
#include <unordered_map>
#include<thread>
#include<chrono>

#include "Graph.h"
#include "utility.h"
#include <iostream>

/*Djikstra
* - Generalisation of BFS to Weighted graph
* - Instead of measuring 'no of steps from source' we measure the 'weighted distance from source' 
* - Frontier is now a priority queue, ordered by distance
* - It is a Greedy algorithm - at each iteration we pick the node with the shortest distance
* - 

*/



void DjikstraFull(DjikstraGraph& dgraph) //Returns a map containing the parents of each cell
{

	int start = *dgraph.start_index();

	//Create the priority queue
	auto cmp = [&dgraph](int a, int b) {return dgraph.cost_so_far[a] > dgraph.cost_so_far[b]; };
	std::priority_queue<int, std::vector<int>, decltype(cmp)> frontier(cmp);

	//Initialise
	//dgraph.pq.push(start);
	frontier.push(start);
	dgraph.parents[start] = start;
	dgraph.cost_so_far[start] = 0;

	//Frontier contains vertices who's neighbours haven't yet been examined
	//Assumes a valid start exists
	dgraph[start].setSeen();

	while (!graph.frontier.empty())
	{

		int curr = front();
		graph.frontier.pop();
		graph[curr].Visited();

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


