#pragma once
#include "node.h"

Node::Node(float x, float y, float w_size, int dim)
	:rect_{ sf::RectangleShape() }, type_{ Type::empty_ }, seen_{ false }
{
	rect_.setPosition(x, y);
	rect_.setSize(sf::Vector2f(w_size / dim, w_size / dim)); //Grid is 20x20?
	rect_.setFillColor(sf::Color::White); //set Nodes to white
	rect_.setOutlineColor(sf::Color::Black);
	rect_.setOutlineThickness(1);
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

	case Type::grass_:
	{
		if (type_ != Type::start_ && type_ != Type::end_) { rect_.setFillColor(sf::Color::Green); } //Don't change color of start or end nodes
		type_ = Type::grass_;
		break;
	}
	}
}

//Flags that a Node has been placed in the queue during a search. 
void Node::setSeen()
{
	if (type_ != Type::start_ && type_ != Type::end_) { rect_.setFillColor(sf::Color::Cyan); } //Only change color if node isn't start or end
	seen_ = true;
}

//Change color to indicate that a Node has been processed in a search
void Node::Visited()
{
	//Only change color if node isn't start or end
	if (type_ != Type::start_ && type_ != Type::end_)
	{
		rect_.setFillColor(sf::Color::Magenta);
	} 
}

void Node::reset()
{
	rect_.setFillColor(sf::Color::White);
	type_ = Type::empty_;
	seen_ = false;
}