#include <SFML/Graphics.hpp>
#include <iostream>

#include "graph.h"
#include "utility.h"
#include "bfs.h"





int main()
{
    constexpr int w_size{ 800 }; //Size of the window
    sf::RenderWindow window(sf::VideoMode(w_size, w_size), "Path Solver", sf::Style::Titlebar | sf::Style::Close); //Construct window

    //Setup the grid
    constexpr int dim{ 20 };
    Graph graph(dim);
    graph.fill(w_size);

    bool done = false;
    bool toggle_step;

    //Game Loop
    while (window.isOpen())
    {

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) //User can use left click to place walls
        {
            if (done) //If user clicks after a search, automatically reset the board
            {
                for (int i = 0; i < graph.size(); ++i) { graph[i].reset(); }
                done = false;
            }
            if (sf::Mouse::getPosition(window).x >=0 && sf::Mouse::getPosition(window).y >= 0 && sf::Mouse::getPosition(window).x < w_size && sf::Mouse::getPosition(window).y < w_size)
            {
                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                auto& curr_node = graph[row_no*dim  + col_no]; //Node (x,y) can be indexed as y*row_size + x. Don't want a copy

                curr_node.setWall(); //User cannot place walls on starting location

            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) //Press R to reset the grid
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
                        if (in_bounds(window,w_size)) //Clamp...
                        {
                            auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked node 
                            graph.addStart(row_no * dim + col_no);
                        }
                    }

                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) //User can press 2 to change the end location to mouse position
                    {
                            if (in_bounds(window, w_size)) //Clamp...
                            {
                                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                                graph.addEnd(row_no * dim + col_no);
                            }
                    }

                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //User can press Space to run the BFS
                    {
                        //Force a reset if search has already been completed
                        if (done)
                        {
                            std::cout << "press R to reset\n";
                        }
                        if (graph.start()>=0 && graph.end()>=0 && !done)
                        {     

                            //int start = graph.start();

                            //graph.frontier.push(start);
                            //graph.parents[start] = -1;
                            //graph.distance[start] = 0;

                            //int next = bfs_step(graph);
                            //if (next ==-1)
                            //{
                            //    done = true;
                            //    draw_path(graph);
                            //}
                            bfs_full(graph);
                            draw_path(graph);

                        }
                        else std::cout << "Please place a start and end location before searching\n";
                    }
                    break;
                }

            }

        }

        //Draw
        window.clear(); //Clear Screen so contents from previous frame doesn't interfere with current frame
        for (int i = 0; i < graph.size();++i) graph[i].draw(window, sf::RenderStates::Default);
        window.display(); //Swap buffers and display on screen
    }

    return 0;
}