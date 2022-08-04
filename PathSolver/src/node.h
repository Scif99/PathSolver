#pragma once
#include <SFML/Graphics.hpp>


class Node : sf::Drawable 
{
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

	Node(float x, float y, int sz); //Constructor takes an x position, y position, and the size (x and y will both take this value)
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override { target.draw(rect_, states); }; 

	bool isType(Type t) const { return type_ == t; }
	void setType(Type t);

	void setSeen();
	bool isSeen() const { return seen_; }

	void Visited(); //Simply adds color to a Node (except if the Node is the start/end)

	void reset();
private:
	Type type_;
	bool seen_; //Represents whether a node has been flagged for processing (i.e added to the priority queue)
	sf::RectangleShape rect_;
};


