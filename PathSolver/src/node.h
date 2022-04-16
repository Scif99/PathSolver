#pragma once
#include <SFML/Graphics.hpp>




class Node : sf::RectangleShape {
public:

	Node(float x, float y, float w_size); //Constructor takes an x position, y position and the size of the window
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //The draw function is inherited from the sf::Drawable class
	void setWall();
	void setStart();
	void setEnd();
	void setSeen();
	void setPath();
	void reset();

	bool isStart() const { return is_start_; }
	bool isEnd() const { return is_end_; }
	bool isSeen() const { return is_seen_; }
	bool isPath() const { return is_path_; }
	bool isWall() const { return is_wall_; }

	
private:
	sf::RectangleShape rect_;
	bool is_wall_;
	bool is_start_;
	bool is_end_;
	bool is_seen_;
	bool is_path_;
	enum class Type
	{
		empty_,
		wall_,
		start_,
		end_,
		seen_,
		path_,
	};
};




Node::Node(float x, float y, float w_size)
	:rect_{ sf::RectangleShape() }, is_wall_{ false }, is_start_{ false }, is_end_{false}, is_seen_{false}, is_path_{false}
{
	rect_.setPosition(x, y);
	rect_.setSize(sf::Vector2f(w_size / 20, w_size / 20)); //Grid is 20x20?
	rect_.setFillColor(sf::Color::White); //set Nodes to white
	rect_.setOutlineColor(sf::Color::Black);
	rect_.setOutlineThickness(1);
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect_, states);
}

void Node::setStart()
{
	rect_.setFillColor(sf::Color::Blue);
	is_start_ = true;
}

void Node::setWall()
{

	rect_.setFillColor(sf::Color::Black);
	is_wall_ = true;
}

void Node::setEnd()
{
	rect_.setFillColor(sf::Color::Red);
	is_end_ = true;
}

void Node::setSeen()
{
	is_seen_ = true;
	rect_.setFillColor(sf::Color::Cyan);	
}


void Node::setPath()
{
	if (!is_start_ && !is_end_)
	{
		is_path_ = true;
		rect_.setFillColor(sf::Color::Yellow);
	}
}

void Node::reset()
{
	rect_.setFillColor(sf::Color::White);
	is_wall_ = false;
	is_start_ = false;
	is_end_ = false;
	is_seen_ = false;
	is_path_ = false;
}

