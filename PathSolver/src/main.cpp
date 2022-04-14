#include <SFML/Graphics.hpp>
#include <iostream>

#include "graph.h"
#include "utility.h"
#include "bfs.h"



int main()
{
    int w_size = 800; //Size of the window
    sf::RenderWindow window(sf::VideoMode(w_size, w_size), "Path Solver", sf::Style::Titlebar | sf::Style::Close); //Construct window

    //Create the grid
    int dim = 20; //dimensions of grid

    Graph graph(dim);
    graph.fill(w_size);

    //Game Loop
    while (window.isOpen())
    {

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) //User can use left click to place walls
        {

            if (sf::Mouse::getPosition(window).x >=0 && sf::Mouse::getPosition(window).y >= 0 && sf::Mouse::getPosition(window).x < w_size && sf::Mouse::getPosition(window).y < w_size)
            {
                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                auto& curr_node = graph[row_no*dim  + col_no]; //Cell (x,y) can be written as y*row_size + x. Don't want a copy

                if(!curr_node.isStart()) curr_node.setWall(); //User cannot place walls on starting location

            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) //Press R to reset the grid
        {
            for (int i = 0; i < graph.size();++i)
            {
                graph[i].reset();
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

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) //User can press 1 to change the start location to mouse position
                    {
                        if (sf::Mouse::getPosition(window).x >= 0 && sf::Mouse::getPosition(window).y >= 0 && sf::Mouse::getPosition(window).x < w_size && sf::Mouse::getPosition(window).y < w_size) //Clamp...
                        {
                            auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                            auto& curr_node = graph[row_no * dim + col_no]; //Cell (x,y) can be written as y*row_size + x. Don't want a copy

                            for (int i = 0; i < graph.size();++i) //Reset the previous starting cell
                            {
                                if (graph[i].isStart()) graph[i].reset();
                            }

                            curr_node.setStart();
                            std::cout << "Placed start at " << row_no * dim + col_no << '\n';
                        }
                    }

                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) //User can press 2 to change the end location to mouse position
                    {
                            if (sf::Mouse::getPosition(window).x >= 0 && sf::Mouse::getPosition(window).y >= 0 && sf::Mouse::getPosition(window).x < w_size && sf::Mouse::getPosition(window).y < w_size) //Clamp...
                            {
                                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                                auto& curr_node = graph[row_no*dim + col_no]; //Cell (x,y) can be written as y*row_size + x. Don't want a copy

                                for (int i = 0; i < graph.size();++i) //Reset the previous starting cell
                                {
                                    if (graph[i].isEnd()) graph[i].reset();
                                }
                                std::cout << "Placed end at " << row_no * dim + col_no << '\n';
                                curr_node.setEnd();
                            }
                    }

                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //User can press Space to run the BFS
                    {

                        auto t1 = std::chrono::high_resolution_clock::now(); //time before

                        if (graph.has_end() && graph.has_start())
                        {
                            std::unordered_map<int, int> parents = bfs(graph);
                            draw_path(parents, graph);

                            auto t2 = std::chrono::high_resolution_clock::now(); //time after
                            auto sec = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1); /* Getting number of milliseconds as an integer. */

                            std::cout << sec.count() << "s\n"; //Log time
                        }
                        else std::cout << "Please place a start and end location before searching\n";
                    }

                    break;
                }


            }

        }

        //Draw
        window.clear(); //Clear Screen so contents from previous frame isnt 
        for (int i = 0; i < graph.size();++i) graph[i].draw(window, sf::RenderStates::Default);
        window.display(); //Swap buffers and display on screen
    }

    return 0;
}