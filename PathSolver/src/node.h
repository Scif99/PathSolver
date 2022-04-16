#pragma once
#include <SFML/Graphics.hpp>




class Node : sf::RectangleShape {
public:

	Node(float x, float y, float w_size, int dim); //Constructor takes an x position, y position and the size of the window
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; //The draw function is inherited from the sf::Drawable class
	void setWall();
	void setStart();
	void setEnd();
	void setSeen();
	void setPath();
	void reset();

	bool isStart() const { return type_== Type::start_; }
	bool isEnd() const { return type_==Type::end_; }
	bool isPath() const { return type_ == Type::path_; }
	bool isWall() const { return type_ == Type::wall_; }
	bool isSeen() const { return is_seen_; }

	enum class Type
	{
		empty_,
		wall_,
		start_,
		end_,
		path_,
	};
	
private:
	sf::RectangleShape rect_;
	Type type_;
	bool is_seen_;
};




Node::Node(float x, float y, float w_size, int dim)
	:rect_{ sf::RectangleShape() }, type_{ Type::empty_ }, is_seen_{ false }
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
	rect_.setFillColor(sf::Color::Cyan);	
	is_seen_ = true;
}

void Node::setPath()
{
	rect_.setFillColor(sf::Color::Yellow);
	type_ = Type::path_;
}

void Node::reset()
{
	rect_.setFillColor(sf::Color::White);
	type_ = Type::empty_;
	is_seen_ = false;

}

