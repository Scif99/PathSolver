#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

//Checks whether the mouse position lies within the window
bool mouse_in_bounds(const sf::Window& win)
{
	return sf::Mouse::getPosition(win).x >= 0 &&
		sf::Mouse::getPosition(win).y >= 0 &&
		sf::Mouse::getPosition(win).x < win.getSize().x &&
		sf::Mouse::getPosition(win).y < win.getSize().x;
}

//Returns the row and column number of the cell that the cursor is currently at
std::pair<int, int> getCoords(const sf::Window& win, int grid_dim)
{

	auto x = sf::Mouse::getPosition(win).x;
	auto y = sf::Mouse::getPosition(win).y;
	int unit = win.getSize().x / grid_dim; //grid is dim cells wide

	int col_no = x / unit;
	int row_no = y / unit;
	return { row_no,col_no };
}

void print_controls() 
{
	std::cout << "Controls\n\n";

	std::cout << "1: Place start node\n";
	std::cout << "2: Place end node\n";
	std::cout << "Left-Click: Place walls\n";
	std::cout << "Right-Click: Place grass(grass nodes have a cost of 5)\n";
	std::cout << "SPACE : Runs the algorithm\n";
	std::cout << "R : Resets the grid\n";
	std::cout << "T : Switches between step / full mode\n\n";

	std::cout << "Algorithms\n\n";

	std::cout << "B : BFS\n";
	std::cout << "G : Greedy BFS\n";
	std::cout << "A : A*\n";
	std::cout << "D : Djikstra\n";
}