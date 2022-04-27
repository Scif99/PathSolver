#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <memory>
#include <utility>

#include "graph.h"
#include "utility.h"

int main()
{
    print_controls();

    constexpr auto w_size{ 800 }; //Size of the window
    sf::RenderWindow window(sf::VideoMode(w_size, w_size), "Path Solver", sf::Style::Titlebar | sf::Style::Close); //Construct window

    //Set up the grid
    constexpr auto dim{ 20 }; //Number of rows/columns

    std::unique_ptr<Graph> pgraph = std::make_unique<BFSGraph>(BFSGraph(dim));
    pgraph->fill(w_size); //Fill the graph with nodes (cells)

    auto done{ false }; //Has a search been completed?
    auto toggle_step{ false }; //Are we in step mode?
    auto stepping {false }; //Is the algo running in real time?

    //Game Loop
    while (window.isOpen())
    {
        //If in step mode
        if(stepping && !done)
        {
            int next = pgraph->step();
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); //Pause
            if (next == -1)
            {
                pgraph->drawPath();
                done = true;
                stepping = false;
            }
        }

        //User can use left click to place walls
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !stepping) //User can use left click to place walls
        {
            //If user clicks after a search, automatically reset the board first
            if (done)
            {
                pgraph->reset();
                done = false;
            }
            if (mouse_in_bounds(window,w_size))
            {
                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                pgraph->addWall(row_no * dim + col_no);
            }
        }

        //User can use left click to place Grass with weight 5 (note this makes no difference for BFS)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !stepping) 
        {
            //If user clicks after a search, automatically reset the board first
            if (done) 
            {
                pgraph->reset();
                done = false;
            }
            if (mouse_in_bounds(window, w_size))
            {
                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                pgraph->addGrass(row_no * dim + col_no);
            }
        }

        //Process Events    
        sf::Event evnt;
        while (window.pollEvent(evnt)) //Check for any events. Could be Multiple hence to loop
        {
            switch (evnt.type)
            {
                //Close window
                case  sf::Event::Closed:
                {
                    window.close();
                    break;
                }

                case sf::Event::KeyPressed:
                {
                    //User can Press R to manually reset the grid to a clean slate
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                    {
                        pgraph->reset();
                        done = false;
                        stepping = false;
                    }

                    //User can press 1 to change the start location to mouse position
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) 
                    {
                        //If user clicks after a search, automatically reset the board
                        if (done) 
                        {
                            pgraph->reset();
                            done = false;
                        }
                        if (mouse_in_bounds(window,w_size)) //Clamp...
                        {
                            auto [row_no, col_no] = getCoords(window, w_size, dim); 
                            pgraph->addStart(row_no * dim + col_no);
                            //std::cout << "Placed start node at " << row_no * dim + col_no << '\n';
                        }
                    }

                    //User can press 2 to change the end location to mouse position
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) 
                    {
                        //If user clicks after a search, automatically reset the board
                        if (done) 
                        {
                            pgraph->reset();
                            done = false;
                        }
                        if (mouse_in_bounds(window, w_size)) //Clamp...
                            {
                                auto [row_no, col_no] = getCoords(window, w_size, dim); 
                                pgraph->addEnd(row_no * dim + col_no);
                                //std::cout << "Placed end node at " << row_no * dim + col_no << '\n';
                            }
                    }

                    //User can press space to run the full search.
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
                    {
                        //Force a reset if search has already been completed.
                        if (done)
                        {
                            std::cout << "Search already completed. Press r to reset\n";
                            break;
                        }
                        //Prompt user if they haven't placed a start and end.
                        if (!pgraph->start_index() || !pgraph->end_index()) 
                        {
                            std::cout << "please place a start and end location before searching\n";
                            break;
                        }
                        //Run the search, depending on whether user is in full or step mode.
                        if (!toggle_step)
                        {
                            pgraph->run();
                            pgraph->drawPath();

                            done = true; //Flag that the search has been completed.
                        }
                        else
                        {
                            stepping = true; //The algorithm should now run step-by-step 
                        }
                    }

                    //User can press T to toggle between step and full modes
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
                    {
                        toggle_step = !toggle_step;
                        std::string mode = toggle_step ? "Step " : "Full ";
                        std::cout << "Switched to " << mode << " mode\n";
                    }

                    //User can press B to switch to BFS Search
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
                    {
                        pgraph->reset();
                        done = false;
                        stepping = false;
                        pgraph = std::move(std::make_unique<BFSGraph>(BFSGraph(dim)));
                        pgraph->fill(w_size);
                        std::cout << "Switched to BFS Search\n";

                    }

                    //User can press D to switch to Djikstra
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    {
                        pgraph->reset();
                        done = false;
                        stepping = false;
                        pgraph = std::move(std::make_unique<DjikstraGraph>(DjikstraGraph(dim)));
                        pgraph->fill(w_size);
                        std::cout << "Switched to Djikstra\n";

                    }

                    //User can press G to switch to Greedy Search
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
                    {
                        pgraph->reset();
                        done = false;
                        stepping = false;
                        pgraph = std::move(std::make_unique<GreedyGraph>(GreedyGraph(dim)));
                        pgraph->fill(w_size);
                        std::cout << "Switched to Greedy Search\n";
                    
                    }

                    //User can press G to switch to A* Search
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    {
                        pgraph->reset();
                        done = false;
                        stepping = false;
                        pgraph = std::move(std::make_unique<AstarGraph>(AstarGraph(dim)));
                        pgraph->fill(w_size);
                        std::cout << "Switched to A*\n";

                    }

                    break;
                }

            }

        }

        //Draw
        window.clear(); //Clear Screen so contents from previous frame doesn't interfere with current frame
        for (const auto& node : *pgraph) node.draw(window, sf::RenderStates::Default);
        window.display(); //Swap buffers and display on screen
    }

    return 0;
}