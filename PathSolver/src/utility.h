#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

//Checks whether the mouse position lies within the window
bool mouse_in_bounds(const sf::Window& win, int win_size)
{
	return sf::Mouse::getPosition(win).x >= 0 &&
		sf::Mouse::getPosition(win).y >= 0 &&
		sf::Mouse::getPosition(win).x < win_size &&
		sf::Mouse::getPosition(win).y < win_size;
}

//Returns the row and column number of the cell that the cursor is currently at
std::pair<int, int> getCoords(const sf::Window& win, int win_size, int grid_dim)
{

	auto x = sf::Mouse::getPosition(win).x;
	auto y = sf::Mouse::getPosition(win).y;
	int unit = win_size / grid_dim; //grid is dim cells wide

	int col_no = x / unit;
	int row_no = y / unit;
	return { row_no,col_no };
}

//Prints message to console indicating a switch of modes
void printToggled(bool& toggle, std::ostream& os)
{
	toggle = !toggle;
	std::string mode = toggle ? "Step " : "Full ";
	os << "Switched to " << mode << " mode\n";
}

