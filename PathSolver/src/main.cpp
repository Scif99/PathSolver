#include <SFML/Graphics.hpp>
#include <iostream>

#include "cell.h"
#include "utility.h"



int main()
{
    int w_size = 800; //Size of the window
    sf::RenderWindow window(sf::VideoMode(w_size, w_size), "Path Solver", sf::Style::Titlebar | sf::Style::Close); //Construct window

    //Create the grid
    int dim = 20; //dimensions of grid
    std::vector<Cell> v_cells; //Note that the grid is implemented as a single vector rather than a nested one.


    //Fill the grid
    for (int i = 0; i < dim; ++i)
    {
        for (int j = 0;j < dim;++j)
        {
            v_cells.push_back(Cell{ j * w_size / dim, i * w_size / dim, w_size });//Note we want vector to go along columns --->, hence the ordering of i and j
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
                auto& curr_cell = v_cells[dim * col_no + row_no]; //Cell (x,y) can be written as y*row_size + x. Don't want a copy

                if(!curr_cell.isStart()) curr_cell.setWall(); //User cannot place walls on starting location
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) //User can use right click to change the starting location
        {
            if (sf::Mouse::getPosition(window).x >= 0 && sf::Mouse::getPosition(window).y >= 0 && sf::Mouse::getPosition(window).x < w_size && sf::Mouse::getPosition(window).y < w_size) //Clamp...
            {
                auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                auto& curr_cell = v_cells[dim * col_no + row_no]; //Cell (x,y) can be written as y*row_size + x. Don't want a copy

                for (auto& cell : v_cells) //Reset the previous starting cell
                {
                    if (cell.isStart()) cell.reset(); 
                }

                curr_cell.setStart();
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

                case sf::Event::TextEntered:
                {
                    if (evnt.text.unicode < 128 && evnt.text.unicode != 8) //exclude backspace
                        std::cout << "ASCII character typed: " << static_cast<char>(evnt.text.unicode) << '\n';
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    //if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) //Left click to place walls
                    //{
                    //    auto [row_no, col_no] = getCoords(window, w_size, dim); //Get indices of the clicked cell
                    //    auto& curr_cell = v_cells[dim * col_no + row_no]; //Cell (x,y) can be written as y*row_size + x. Don't want a copy

                    //    curr_cell.setWall();
                    //}

                    //Right click to set start location

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