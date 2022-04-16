#pragma once
#include <SFML/Graphics.hpp>

class Node : sf::RectangleShape {

private:
	sf::RectangleShape rect_;
	bool seen_;
	enum class Type
	{
		empty_,
		wall_,
		start_,
		end_,
		path_,
		visited_,
	};
	Type type_;

public:

	Node(float x, float y, float w_size, int dim); //Constructor takes an x position, y position and the size of the window
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; //The draw function is inherited from the sf::Drawable class
	void setWall();
	void setStart();
	void setEnd();
	void setSeen();
	void setPath();
	void setVisited();
	void reset();

	bool isStart() const { return type_ == Type::start_; }
	bool isEnd() const { return type_ == Type::end_; }
	bool isPath() const { return type_ == Type::path_; }
	bool isWall() const { return type_ == Type::wall_; }
	bool isSeen() const { return seen_; } 


};

Node::Node(float x, float y, float w_size, int dim)
	:rect_{ sf::RectangleShape() }, type_{ Type::empty_ }, seen_{ false }
{
	rect_.setPosition(x, y);
	rect_.setSize(sf::Vector2f(w_size / dim, w_size / dim)); //Grid is 20x20?
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
	type_ = Type::start_;
}

void Node::setWall()
{
	rect_.setFillColor(sf::Color::Black);
	type_ = Type::wall_;
}

void Node::setEnd()
{
	rect_.setFillColor(sf::Color::Red);
	type_ = Type::end_;
}

void Node::setSeen()
{
	if (type_ != Type::start_ && type_ != Type::end_) { rect_.setFillColor(sf::Color::Cyan); } //Only change color if node isn't start or end
	seen_ = true;
}

void Node::setVisited()
{
	if (type_ != Type::start_ && type_ != Type::end_) rect_.setFillColor(sf::Color::Magenta);  //Only change color if node isn't start or end
}

void Node::setPath()
{
	if (type_ != Type::start_ && type_ != Type::end_) { rect_.setFillColor(sf::Color::Yellow); } //Only change color if node isn't start or end
	type_ = Type::path_;
}



void Node::reset()
{
	rect_.setFillColor(sf::Color::White);
	type_ = Type::empty_;
	seen_ = false;
}



