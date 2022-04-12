#pragma once
#include <SFML/Graphics.hpp>

//Log some info...

void clamp(int& pos, int low, int high)
{
	if (pos < low) pos = low;
	if (pos > high) pos = high;
}

std::pair<int, int> getCoords(const sf::Window& win, int win_size, int grid_dim)
{

	auto x = sf::Mouse::getPosition(win).x;
	auto y = sf::Mouse::getPosition(win).y;
	int unit = win_size / grid_dim; //grid is dim cells wide
	clamp(x, 0, win_size);
	clamp(y, 0, win_size);
	int col_no = x / unit;
	int row_no = y / unit;
	return { row_no,col_no };
}
