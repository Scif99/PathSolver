#pragma once
#include <SFML/Graphics.hpp>

//Log some info...

std::pair<int, int> getCoords(const sf::Window& win, int win_size, int dim)
{
	auto x = sf::Mouse::getPosition(win).x;
	auto y = sf::Mouse::getPosition(win).y;
	int unit = win_size / dim; //grid is dim cells wide
	int row_no = x / unit;
	int col_no = y / unit;
	return { row_no,col_no };
}