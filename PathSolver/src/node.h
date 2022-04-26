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

	Node(float x, float y, int w_size, int dim); //Constructor takes an x position, y position, the size of the window, and the dimensions of the grid
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(rect_, states); }; //The draw function is inherited from the sf::Drawable class

	bool isType(Type t) const { return type_ == t; }
	void setType(Type t);

	void setSeen();
	bool isSeen() const { return seen_; }

	void Visited(); //Simply adds color to a Node (except if the Node is the start/end)

	void reset();
private:
	sf::RectangleShape rect_;
	bool seen_; //Represents whether a node has been flagged for processing (i.e added to the priority queue)
	Type type_;
};


