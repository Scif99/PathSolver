#include <SFML/Graphics.hpp>
#include <iostream>

#include "graph.h"
#include "utility.h"
#include "bfs.h"
#include "djikstra.h"


int main()
{
    constexpr int w_size{ 800 }; //Size of the window
    sf::RenderWindow window(sf::VideoMode(w_size, w_size), "Path Solver", sf::Style::Titlebar | sf::Style::Close); //Construct window

    //Setup the grid
    constexpr int dim{ 20 };
    //BFSGraph graph(dim);
    DjikstraGraph graph(dim);
    graph.fill(w_size);

    bool done = false;
    bool toggle_step = false;

    //Game Loop
    while (window.isOpen())
    {

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

        //User can use left click to place walls
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) //User can use left click to place walls
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

        //Press R to reset the grid
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) 
        {
            graph.reset();
            done = false;
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
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) //User can press 1 to change the start location to mouse position
                    {
                        if (mouse_in_bounds(window,w_size)) //Clamp...
                        {
                            auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked node 
                            graph.addStart(row_no * dim + col_no);
                        }
                    }

                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) //User can press 2 to change the end location to mouse position
                    {
                            if (mouse_in_bounds(window, w_size)) //Clamp...
                            {
                                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                                graph.addEnd(row_no * dim + col_no);
                            }
                    }

                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //User can press Space to run the BFS
                    {
                        //Force a reset if search has already been completed
                        if (done) std::cout << "press R to reset\n";
                        
                        else if (!graph.start_index() || !graph.end_index()) std::cout << "Please place a start and end location before searching\n";
                        else
                        {     
                            if (toggle_step == false) 
                            {
                                //bfs_full(graph);
                                DjikstraFull(graph);
                                draw_path(graph);
                                done = true;
                            }
                            else 
                            {
                                //int next = bfs_step(graph);

                                //if (next == -1)
                                //{
                                //    draw_path(graph);
                                //    done = true;
                                //}
                            }
                        }
                        
                    }

                    //User can press T to switch between step and full modes
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
                    {
                        toggle_step = !toggle_step;
                        std::string mode = toggle_step ? "Step " : "Full ";
                        std::cout << "Switched to " << mode << " mode\n";
                        done = true;
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