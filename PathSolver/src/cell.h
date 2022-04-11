#pragma once
#include <SFML/Graphics.hpp>

class Cell : sf::RectangleShape {
public:

	Cell(int x, int y, int w_size); //Constructor takes an x position, y position and the size of the window
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //The draw function is inherited from the sf::Drawable class
	void setWall();
	void setStart();
	void setEnd();
	void reset();

	bool isStart() const { return is_start_; }
	bool isEnd() const { return is_end_; }
private:
	sf::RectangleShape rect_;
	bool is_wall_;
	bool is_start_;
	bool is_end_;

};




Cell::Cell(int x, int y, int w_size)
	:rect_{ sf::RectangleShape() }, is_wall_{ false }, is_start_{ false }, is_end_{false}
{
	rect_.setPosition(x, y);
	rect_.setSize(sf::Vector2f(w_size / 3, w_size / 3)); //Grid is 3x3
	rect_.setFillColor(sf::Color::White); //set cells to white
	rect_.setOutlineColor(sf::Color::Black);
	rect_.setOutlineThickness(1);

}

void Cell::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect_, states);
}

void Cell::setStart()
{
	rect_.setFillColor(sf::Color::Blue);
	is_start_ = true;
}

void Cell::setWall()
{
	rect_.setFillColor(sf::Color::Black);
	is_wall_ = true;
}

void Cell::setEnd()
{
	rect_.setFillColor(sf::Color::Red);
	is_end_ = true;
}

void Cell::reset()
{
	rect_.setFillColor(sf::Color::White);
	is_wall_ = false;
	is_start_ = false;
	is_end_ = false;
}

