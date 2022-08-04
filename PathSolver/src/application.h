#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <memory>
#include <utility>

#include "utility.h"
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
		: r_window_{ window } 
	{
		p_graph_ = factory<BFSGraph>(m_grid_dim_);
 
	}

	//Non-copyable
	Application(const Application& other) = delete;
	Application& operator=(const Application& other) = delete;
	//Non-moveable
	Application(Application&& other) = delete;
	Application& operator=(Application&& other) = delete;

    ~Application() = default;

	void run();

	void handleInput();
	void handleEvents();
	void update() {};
	void draw();



private:
	sf::RenderWindow& r_window_;
	Mode e_mode_{Mode::FULL};
	std::unique_ptr<Graph> p_graph_;
    int m_grid_dim_{ 20 };
    bool b_running_{false};
    bool b_done_{ false };
};

//Main loop
void Application::run()
{
	while(true)
	{
		handleInput();
		handleEvents();
		update();
		draw();
	}
}

void Application::handleInput()
{
    //If in step mode
    if (b_running_ && !b_done_)
    {
        int next{ p_graph_->step() };
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); //60fps
        if (next == -1)
        {
            p_graph_->drawPath();
            b_done_ = true;
            b_running_ = false;
        }
    }

    //User can use left click to place walls
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !b_running_) //User can use left click to place walls
    {
        //If user clicks after a search, automatically reset the board first
        if (b_done_)
        {
            p_graph_->reset();
            b_done_ = false;
        }
        if (mouse_in_bounds(r_window_))
        {
            auto [row_no, col_no] = getCoords(r_window_, m_grid_dim_); //Get indices of the clicked cell
            p_graph_->addWall(row_no * m_grid_dim_ + col_no);
        }
    }

    //User can use left click to place Grass with weight 5 (note this makes no difference for BFS)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !b_running_)
    {
        //If user clicks after a search, automatically reset the board first
        if (b_done_)
        {
            p_graph_->reset();
            b_done_= false;
        }
        if (mouse_in_bounds(r_window_))
        {
            auto [row_no, col_no] = getCoords(r_window_, m_grid_dim_); //Get indices of the clicked cell
            p_graph_->addGrass(row_no * m_grid_dim_ + col_no);
        }
    }
}



void Application::handleEvents()
{
    //Process Events    
    sf::Event evnt;
    while (r_window_.pollEvent(evnt)) //Check for any events. Could be Multiple hence to loop
    {
        switch (evnt.type)
        {
            //Close window
        case  sf::Event::Closed:
        {
            r_window_.close();
            break;
        }

        case sf::Event::KeyPressed:
        {
            //User can Press R to manually reset the grid to a clean slate
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                p_graph_->reset();
                b_done_ = false;
                b_running_= false;
            }

            //User can press 1 to change the start location to mouse position
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            {
                //If user clicks after a search, automatically reset the board
                if (b_done_)
                {
                    p_graph_->reset();
                    b_done_  = false;
                }
                if (mouse_in_bounds(r_window_)) //Clamp...
                {
                    auto [row_no, col_no] = getCoords(r_window_, m_grid_dim_);
                    p_graph_->addStart(row_no * m_grid_dim_ + col_no);
                    //std::cout << "Placed start node at " << row_no * dim + col_no << '\n';
                }
            }

            //User can press 2 to change the end location to mouse position
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            {
                //If user clicks after a search, automatically reset the board
                if (b_done_)
                {
                    p_graph_->reset();
                    b_done_ = false;
                }
                if (mouse_in_bounds(r_window_)) //Clamp...
                {
                    auto [row_no, col_no] = getCoords(r_window_, m_grid_dim_);
                    p_graph_->addEnd(row_no * m_grid_dim_ + col_no);
                    //std::cout << "Placed end node at " << row_no * dim + col_no << '\n';
                }
            }

            //User can press space to run the full search.
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                //Force a reset if search has already been completed.
                if (b_done_)
                {
                    std::cout << "Search already completed. Press r to reset\n";
                    break;
                }
                //Prompt user if they haven't placed a start and end.
                if (!p_graph_->start_index() || !p_graph_->end_index())
                {
                    std::cout << "please place a start and end location before searching\n";
                    break;
                }
                //Run the search, depending on whether user is in full or step mode.
                if (e_mode_ == Mode::FULL)
                {
                    p_graph_->run();
                    p_graph_->drawPath();

                    b_done_ = true; //Flag that the search has been completed.
                }
                else
                {
                    b_running_ = true; //The algorithm should now run step-by-step 
                }
            }

            //User can press T to toggle between step and full modes
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            {
                e_mode_ = e_mode_ == Application::Mode::FULL ? Application::Mode::STEP : Application::Mode::FULL;
                std::string mode = e_mode_== Application::Mode::STEP ? "Step " : "Full ";
                std::cout << "Switched to " << mode << " mode\n";
            }

            //User can press B to switch to BFS Search
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            {
                p_graph_ = factory<BFSGraph>(m_grid_dim_ );

                std::cout << "Switched to BFS Search\n";

            }

            //User can press D to switch to Djikstra
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                p_graph_ = factory<DjikstraGraph>(m_grid_dim_);
                b_done_ = false;
                b_running_ = false;
                std::cout << "Switched to Djikstra\n";

            }

            //User can press G to switch to Greedy Search
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
            {
                p_graph_ = factory<BFSGraph>(m_grid_dim_);
                b_done_ = false;
                b_running_ = false;
                std::cout << "Switched to Greedy Search\n";

            }

            //User can press G to switch to A* Search
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                p_graph_ = factory<BFSGraph>(m_grid_dim_);
                b_done_ = false;
                b_running_ = false;
                std::cout << "Switched to A*\n";

            }
            break;
        }

        }

    }
}



void Application::draw()
{
    r_window_.clear(); //Clear Screen so contents from previous frame doesn't interfere with current frame
    for (const auto& node : *p_graph_) node.draw(r_window_, sf::RenderStates::Default);
    r_window_.display(); //Swap buffers and display on screen
}