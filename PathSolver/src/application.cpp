#include "application.h"
#include "utility.h"
//Main loop
void Application::run()
{
    while (true)
    {
        if (!b_running_)
        {
            handleInput();
            handleEvents();
        }
        update();
        draw();
    }
}

void Application::update()
{
    if (!b_running_) return;

    if (e_mode_ == Mode::STEP)
    {
        int next{ p_graph_->step() };
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); //60fps
        if (next == -1)
        {
            p_graph_->drawPath();
            b_running_ = false;
            b_completed_ = true;
        }
    }

    else if (e_mode_ == Mode::FULL)
    {
        p_graph_->run();
        p_graph_->drawPath();


        b_completed_ = true;
        b_running_ = false; //Flag that the search has been completed.
    }


}

//Queries Mouse state.
void Application::handleInput()
{
    //User can use left click to place walls
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) //User can use left click to place walls
    {

        if (b_completed_) { reset(); }
        if (mouse_in_bounds(r_window_))
        {
            auto [row_no, col_no] = getCoords(r_window_, m_grid_dim_); //Get indices of the clicked cell
            p_graph_->addWall(row_no * m_grid_dim_ + col_no);
        }
    }

    //User can use left click to place Grass with weight 5 (note this makes no difference for BFS)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {

        if (b_completed_) { reset(); }

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
                reset();
            }

            //User can press 1 to change the start location to mouse position
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            {

                if (b_completed_) { reset(); }

                if (mouse_in_bounds(r_window_))
                {
                    auto [row_no, col_no] = getCoords(r_window_, m_grid_dim_);
                    p_graph_->addStart(row_no * m_grid_dim_ + col_no);
                }
            }

            //User can press 2 to change the end location to mouse position
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            {

                if (b_completed_) { reset(); }
                if (mouse_in_bounds(r_window_)) //Clamp...
                {
                    auto [row_no, col_no] = getCoords(r_window_, m_grid_dim_);
                    p_graph_->addEnd(row_no * m_grid_dim_ + col_no);
                }
            }

            //User can press space to run the full search.
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                //Force a reset if search has already been completed.
                if (b_completed_)
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
                b_running_ = true;

            }

            //User can press T to toggle between step and full modes
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            {
                e_mode_ = e_mode_ == Application::Mode::FULL ? Application::Mode::STEP : Application::Mode::FULL;
                std::string mode = e_mode_ == Application::Mode::STEP ? "Step " : "Full ";
                std::cout << "Switched to " << mode << " mode\n";
            }

            //User can press B to switch to BFS Search
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            {
                p_graph_ = factory<BFSGraph>(m_grid_dim_);
                b_completed_ = false;
                b_running_ = false;
                std::cout << "Switched to BFS Search\n";
            }

            //User can press D to switch to Djikstra
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                p_graph_ = factory<DjikstraGraph>(m_grid_dim_);
                b_completed_ = false;
                b_running_ = false;
                std::cout << "Switched to Djikstra\n";

            }

            //User can press G to switch to Greedy Search
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
            {
                p_graph_ = factory<GreedyGraph>(m_grid_dim_);
                b_completed_ = false;
                b_running_ = false;
                std::cout << "Switched to Greedy Search\n";

            }

            //User can press G to switch to A* Search
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                p_graph_ = factory<AstarGraph>(m_grid_dim_);
                b_completed_ = false;
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

void Application::reset()
{
    p_graph_->reset();
    b_completed_ = false;
    b_running_ = false;
}