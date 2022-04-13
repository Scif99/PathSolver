#include <SFML/Graphics.hpp>
#include <iostream>

#include "cell.h"
#include "utility.h"
#include "bfs.h"



int main()
{
    int w_size = 800; //Size of the window
    sf::RenderWindow window(sf::VideoMode(w_size, w_size), "Path Solver", sf::Style::Titlebar | sf::Style::Close); //Construct window

    //Create the grid
    int dim = 20; //dimensions of grid
    std::vector<Cell> v_cells; //Note that the grid is implemented as a single vector rather than a nested one.
    v_cells.reserve((dim * dim) - 1);

    //Fill the grid
    for (int i = 0; i < dim; ++i) //Row 
    {
        for (int j = 0;j < dim;++j) //Column
        {
            int units = w_size / dim;
            v_cells.push_back(Cell{ j * units, i *units, w_size });//Note we want vector to go along columns --->, hence the ordering of i 
        }

    }



    //Game Loop
    while (window.isOpen())
    {

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) //User can use left click to place walls
        {
            if (sf::Mouse::getPosition(window).x >=0 && sf::Mouse::getPosition(window).y >= 0 && sf::Mouse::getPosition(window).x < w_size && sf::Mouse::getPosition(window).y < w_size)
            {
                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                std::cerr << "row: " << row_no << "col: " << col_no << "index: "<<col_no*dim + row_no <<'\n';
                auto& curr_cell = v_cells[row_no*dim  + col_no]; //Cell (x,y) can be written as y*row_size + x. Don't want a copy

                if(!curr_cell.isStart()) curr_cell.setWall(); //User cannot place walls on starting location
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) //Press R to reset the grid
        {
            for (auto& cell : v_cells)
            {
                cell.reset();
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
                            auto& curr_cell = v_cells[row_no * dim + col_no]; //Cell (x,y) can be written as y*row_size + x. Don't want a copy

                            for (auto& cell : v_cells) //Reset the previous starting cell
                            {
                                if (cell.isStart()) cell.reset();
                            }

                            curr_cell.setStart();
                        }
                    }

                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) //User can press 2 to change the end location to mouse position
                    {
                            if (sf::Mouse::getPosition(window).x >= 0 && sf::Mouse::getPosition(window).y >= 0 && sf::Mouse::getPosition(window).x < w_size && sf::Mouse::getPosition(window).y < w_size) //Clamp...
                            {
                                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                                auto& curr_cell = v_cells[row_no*dim + col_no]; //Cell (x,y) can be written as y*row_size + x. Don't want a copy

                                for (auto& cell : v_cells) //Reset the previous starting cell
                                {
                                    if (cell.isEnd()) cell.reset();
                                }

                                curr_cell.setEnd();
                            }
                    }

                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //User can press Space to run the BFS
                    {
                        std::unordered_map<int,int> parents = bfs(v_cells);
                        draw_path(parents, v_cells);

                    }

                    break;
                }


            }

        }




        window.clear(); //Clear Screen so contents from previous frame isnt 
        for (const auto& cell : v_cells) cell.draw(window, sf::RenderStates::Default);
        window.display(); //Swap buffers and display on screen
    }

    return 0;
}