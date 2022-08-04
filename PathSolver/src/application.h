#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <memory>
#include <utility>

#include "graph.h"

#include "bfs.h"
#include "djikstra.h"
#include "greedy.h"
#include "a_star.h"

template<typename T>
std::unique_ptr<Graph> factory(int dim)
{
	return std::make_unique<T>(dim);
}

class Application
{
public:
	enum class Mode {
		STEP,
		FULL
	};

	Application(sf::RenderWindow& window)
		: r_window_{ window }  {p_graph_ = factory<BFSGraph>(m_grid_dim_);}
    ~Application() = default;

	//Non-copyable
	Application(const Application& other) = delete;
	Application& operator=(const Application& other) = delete;

	//Non-moveable
	Application(Application&& other) = delete;
	Application& operator=(Application&& other) = delete;


	void run();
	void handleInput();
	void handleEvents();
	void update();
	void draw();

    void reset();

private:
	sf::RenderWindow& r_window_;
	Mode e_mode_{Mode::FULL};
	std::unique_ptr<Graph> p_graph_;
    int m_grid_dim_{ 20 };
    bool b_running_{false};
    bool b_completed_{ false }; //Has a search been run?
};

