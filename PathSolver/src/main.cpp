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
                            auto& curr_node = graph[row_no * dim + col_no]; //Node (x,y) can be indexed as y*row_size + x. Don't want a copy

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
                                auto& curr_node = graph[row_no*dim + col_no]; //Node (x,y) can be indexed as y*row_size + x. Don't want a copy

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
                        if (graph.has_start() && graph.has_end())
                        {
                            std::unordered_map<int, int> parents = bfs(graph);
                            draw_path(parents, graph);
                            done = true;
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