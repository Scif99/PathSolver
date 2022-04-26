
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "Graph.h"
#include "utility.h"

int main()
{
    constexpr int w_size{ 800 }; //Size of the window
    sf::RenderWindow window(sf::VideoMode(w_size, w_size), "Path Solver", sf::Style::Titlebar | sf::Style::Close); //Construct window

    //Setup the grid
    constexpr int dim{ 20 }; //Number of rows/columns
    //BFSGraph graph(dim); //Construct an empty graph
    //DjikstraGraph graph(dim);
    //GreedyGraph graph(dim);
    AstarGraph graph(dim);
    
    graph.fill(w_size); //Fill the graph with nodes (cells)

    bool done = false; //Has a search been completed?
    bool toggle_step = false; //Flag to toggle between step/full mode
    bool stepping = false; //Is the algo running in real time

    //Game Loop
    while (window.isOpen())
    {
        //If in step mode
        //TO-DO stepping should block everything else?
        if(stepping && !done)
        {
            int next = graph.step();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            if (next == -1)
            {
                DrawPath(graph);
                done = true;
                stepping = false;
            }
        }

        //User can use left click to place walls
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !stepping) //User can use left click to place walls
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

        //User can use left click to place Grass with weight 5 (note this makes no difference for BFS)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !stepping) 
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
                        graph.reset();
                        done = false;
                        stepping = false;
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
                        if (!graph.start_index() || !graph.end_index()) 
                        {
                            std::cout << "please place a start and end location before searching\n";
                            break;
                        }
                        //Run the search, depending on whether user is in full or step mode.
                        if (!toggle_step)
                        {
                            graph.run();
                            DrawPath(graph);

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

                    ////User can manually exit by pressing escape
                    //else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    //{
                    //    window.close();
                    //    break;
                    //}
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