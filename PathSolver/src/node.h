#pragma once
#include <SFML/Graphics.hpp>


/*
The Node class encapsulates
- The Node class is essentially a wrapper around a rect_shape
- A Node object only knows it's own state. It knows nothing about other nodes.
- The only thing a Node knows about the graph is it's dimensions.

*/

class Node : sf::RectangleShape {
public:
	enum class Type
	{
		empty_,
		wall_,
		start_,
		end_,
		path_,
		visited_,
	};

	Node(float x, float y, float w_size, int dim); //Constructor takes an x position, y position, the size of the window, and the dimensions of the grid
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; //The draw function is inherited from the sf::Drawable class

	bool isType(Type t) const { return type_ == t; }
	void setType(Type t);

	void setSeen();
	bool isSeen() const { return seen_; }

	void reset();
private:
	sf::RectangleShape rect_;
	bool seen_;
	Type type_;
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

void Node::setType(Type t)
{
	switch (t)
	{
		case Type::start_:
		{
			rect_.setFillColor(sf::Color::Blue);
			type_ = Type::start_;
			break;
		}
		case Type::end_:
		{	
			rect_.setFillColor(sf::Color::Red);
			type_ = Type::end_; 
			break;
		}

		case Type::wall_:
		{
			rect_.setFillColor(sf::Color::Black);
			type_ = Type::wall_;
			break;
		}

		case Type::path_:
		{
			if (type_ != Type::start_ && type_ != Type::end_) { rect_.setFillColor(sf::Color::Yellow); } //Don't change color of start or end nodes
			type_ = Type::path_;
			break;
		}

		case Type::visited_:
		{
			if (type_ != Type::start_ && type_ != Type::end_) rect_.setFillColor(sf::Color::Magenta);  //Only change color if node isn't start or end
			//type_ = Type::visited_;
			break;
		}

	
	}
}

void Node::setSeen()
{
	if (type_ != Type::start_ && type_ != Type::end_) { rect_.setFillColor(sf::Color::Cyan); } //Only change color if node isn't start or end
	seen_ = true;
}

void Node::reset()
{
	rect_.setFillColor(sf::Color::White);
	type_ = Type::empty_;
	seen_ = false;
}



