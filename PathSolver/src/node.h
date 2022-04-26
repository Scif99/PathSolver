#pragma once
#include <SFML/Graphics.hpp>

/*
- The Node class is essentially a wrapper around an sf::rectangleShape
- A Node object only knows it's own state. It knows nothing about other nodes.
- The only thing a Node knows about the graph is it's dimensions.

- The seen_ and visited_ data members are purely for visual purposes. 

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
		grass_,
	};

	Node(float x, float y, float w_size, int dim); //Constructor takes an x position, y position, the size of the window, and the dimensions of the grid
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(rect_, states); }; //The draw function is inherited from the sf::Drawable class

	bool isType(Type t) const { return type_ == t; }
	void setType(Type t);

	void setSeen();
	bool isSeen() const { return seen_; }

	void Visited(); //Simply adds color to a Node (except if the Node is the start/end)

	void reset();
private:
	sf::RectangleShape rect_;
	bool seen_;
	Type type_;
};

Node::Node(float x, float y, float w_size, int dim)
	:rect_{ sf::RectangleShape() }, type_{ Node::Type::empty_ }, seen_{ false }
{
	rect_.setPosition(x, y);
	rect_.setSize(sf::Vector2f(w_size / dim, w_size / dim)); //Grid is 20x20?
	rect_.setFillColor(sf::Color::White); //set Nodes to white
	rect_.setOutlineColor(sf::Color::Black);
	rect_.setOutlineThickness(1);
}

void Node::setType(Node::Type t)
{
	switch (t)
	{
	case Node::Type::start_:
	{
		rect_.setFillColor(sf::Color::Blue);
		type_ = Node::Type::start_;
		break;
	}
	case Node::Type::end_:
	{
		rect_.setFillColor(sf::Color::Red);
		type_ = Node::Type::end_;
		break;
	}

	case Node::Type::wall_:
	{
		rect_.setFillColor(sf::Color::Black);
		type_ = Node::Type::wall_;
		break;
	}

	case Node::Type::path_:
	{
		if (type_ != Node::Type::start_ && type_ != Node::Type::end_) { rect_.setFillColor(sf::Color::Yellow); } //Don't change color of start or end nodes
		type_ = Node::Type::path_;
		break;
	}

	case Node::Type::grass_:
	{
		if (type_ != Node::Type::start_ && type_ != Node::Type::end_) { rect_.setFillColor(sf::Color::Green); } //Don't change color of start or end nodes
		type_ = Node::Type::grass_;
		break;
	}
	}
}

//Flags that a Node has been placed in the queue during a search. 
void Node::setSeen()
{
	if (type_ != Node::Type::start_ && type_ != Node::Type::end_) { rect_.setFillColor(sf::Color::Cyan); } //Only change color if node isn't start or end
	seen_ = true;
}

//Change color to indicate that a Node has been processed in a search
void Node::Visited()
{
	//Only change color if node isn't start or end
	if (type_ != Node::Type::start_ && type_ != Node::Type::end_)
	{
		rect_.setFillColor(sf::Color::Magenta);
	}
}

void Node::reset()
{
	rect_.setFillColor(sf::Color::White);
	type_ = Node::Type::empty_;
	seen_ = false;
}
