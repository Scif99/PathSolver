#include <SFML/Graphics.hpp>
#include <chrono>
#include<thread>

#include "graph.h"
#include "utility.h"
#include "bfs.h"
#include "djikstra.h"
#include "astar.h"
#include "greedybfs.h"

int main()
{
    constexpr int w_size{ 800 }; //Size of the window
    sf::RenderWindow window(sf::VideoMode(w_size, w_size), "Path Solver", sf::Style::Titlebar | sf::Style::Close); //Construct window

    //Setup the grid
    constexpr int dim{ 20 }; //Number of rows/columns
    BFSGraph graph(dim);
    //DjikstraGraph graph(dim);
    //AstarGraph graph(dim);
    //GreedyBFS graph(dim);
    graph.fill(w_size);

    bool done = false; //Has a search been completed?
    bool toggle_step = false; //Flag to toggle between step/full mode
    bool stepping = false; //Is the algo running in real time

    //Game Loop
    while (window.isOpen())
    {
        //If in step mode
        if(stepping && !done)
        {
            int next = bfs_step(graph);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            if (next == -1)
            {
                draw_path(graph);
                done = true;
            }
        }

        //User can use left click to place walls
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) //User can use left click to place walls
        {
            if (done) //If user clicks after a search, automatically reset the board
            {
                graph.reset();
                done = false;
            }
            if (mouse_in_bounds(window,w_size))
            {
                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                graph.addWall(row_no * dim + col_no);
            }
        }

        //User can use left click to place Grass with weight 5 (only makes a difference for 'weighted' algorithms)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) 
        {
            if (done) //If user clicks after a search, automatically reset the board
            {
                graph.reset();
                done = false;
            }
            if (mouse_in_bounds(window, w_size))
            {
                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                graph.addGrass(row_no * dim + col_no);
            }
        }


        //Process Events    
        sf::Event evnt;
        while (window.pollEvent(evnt)) //Check for any events. Could be Multiple hence to loop
        {
            switch (evnt.type)
            {
                case  sf::Event::Closed:
                {
                    window.close();
                    break;
                }

                case sf::Event::KeyPressed:
                {
                    //User can Press R to reset the grid
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                    {
                        graph.reset();
                        done = false;
                    }

                    //User can press 1 to change the start location to mouse position
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) 
                    {
                        if (done) //If user clicks after a search, automatically reset the board
                        {
                            graph.reset();
                            done = false;
                        }
                        if (mouse_in_bounds(window,w_size)) //Clamp...
                        {
                            auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked node 
                            graph.addStart(row_no * dim + col_no);
                            std::cout << "Placed start node at " << row_no * dim + col_no << '\n';
                        }
                    }

                    //User can press 2 to change the end location to mouse position
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) 
                    {
                        if (done) //If user clicks after a search, automatically reset the board
                        {
                            graph.reset();
                            done = false;
                        }
                        if (mouse_in_bounds(window, w_size)) //Clamp...
                            {
                                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                                graph.addEnd(row_no * dim + col_no);
                                std::cout << "Placed end node at " << row_no * dim + col_no << '\n';
                            }
                    }

                    //user can press space to run the full search
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
                    {
                        //force a reset if search has already been completed
                        if (done) std::cout << "press r to reset\n";
                        
                        else if (!graph.start_index() || !graph.end_index()) std::cout << "please place a start and end location before searching\n";
                        else
                        { 
                            if (!toggle_step)
                            {
                                bfs_full(graph);
                                //djikstrafull(graph);
                                //astarfull(graph);
                                //greedyfull(graph);
                                draw_path(graph);
                                done = true;
                            }
                            else
                            {
                                int next = bfs_step(graph); //Get index of next node to be processed
                                if (next == -1) //Search is complete
                                {
                                    draw_path(graph);
                                    done = true;
                                }
                            }
                        }
                        
                    }

                    //User can press s to run the search step-by-step
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    {
                    }

                    //User can press T to toggle between step and full modes
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
                    {
                        toggle_step = !toggle_step;
                        std::string mode = toggle_step ? "Step " : "Full ";
                        std::cout << "Switched to " << mode << " mode\n";
                        //done = true;
                    }

                    break;
                }

            }

        }

        //Draw
        window.clear(); //Clear Screen so contents from previous frame doesn't interfere with current frame
        for (const auto& node : graph) node.draw(window, sf::RenderStates::Default);
        window.display(); //Swap buffers and display on screen
    }

    return 0;
}