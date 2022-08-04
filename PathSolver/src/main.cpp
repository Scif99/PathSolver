#include <SFML/Graphics.hpp>

#include "utility.h"
#include "application.h"

int main()
{
    print_controls();

    constexpr auto w_size{ 800 };
    sf::RenderWindow window(sf::VideoMode(w_size, w_size), "Path Solver", sf::Style::Titlebar | sf::Style::Close); //Construct window

    Application app(window);
    app.run();

    return 0;
}