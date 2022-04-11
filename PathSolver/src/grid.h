#pragma once
#include <SFML/Graphics.hpp>


/// <summary>
/// The Cell class represents a 
/// It inherits from sf::Drawable so that we can call draw().
/// </summary>
/// 
class Cell : sf::RectangleShape {
public:

	Cell(int x, int y, int w_size); //Constructor takes an x position, y position and the size of the window
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //The draw function is inherited from the sf::Drawable class
	void setFillColor(sf::Color col);
	void setWall() { is_wall_ = true; }
	
private:
	sf::RectangleShape rect_;
	bool is_wall_;

};

//
//Definitions for Cell class
//
Cell::Cell(int x, int y, int w_size)
	:rect_{ sf::RectangleShape() }, is_wall_{false}
{
	rect_.setPosition(x, y);
	rect_.setSize(sf::Vector2f(w_size / 3, w_size / 3)); //Grid is 3x3
	rect_.setFillColor(sf::Color::White); //set cells to white
	rect_.setOutlineColor(sf::Color::Black);
	rect_.setOutlineThickness(1);

}

void Cell::draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const
{
	target.draw(rect_, states);
}


void Cell::setFillColor(sf::Color col)
{
	rect_.setFillColor(col);
}