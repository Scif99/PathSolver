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

	bool isStart() const { return type_== Type::start_; }
	bool isEnd() const { return type_==Type::end_; }
	bool isSeen() const { return type_ == Type::seen_; }
	bool isPath() const { return type_ == Type::path_; }
	bool isWall() const { return type_ == Type::wall_; }

	
private:
	sf::RectangleShape rect_;
	enum class Type
	{
		empty_,
		wall_,
		start_,
		end_,
		seen_,
		path_,
	};
	Type type_;
};




Node::Node(float x, float y, float w_size)
	:rect_{ sf::RectangleShape() }, type_{Type::empty_}
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
	rect_.setFillColor(sf::Color::Cyan);	
	type_ = Type::seen_;
}


void Node::setPath()
{
	rect_.setFillColor(sf::Color::Yellow);
	type_ = Type::path_;

}

void Node::reset()
{
	rect_.setFillColor(sf::Color::White);
	type_ == Type::start_;
}

