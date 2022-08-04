#include "node.h"
#include <unordered_map>

Node::Node(float x, float y, int sz)
	:type_{ Node::Type::empty_ }, seen_{ false }
{
	rect_.setPosition(x, y);
	rect_.setSize(sf::Vector2f(sz,sz)); //Grid is 20x20?
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