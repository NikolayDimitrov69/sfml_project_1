#include "Player.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game", sf::Style::Close | sf::Style::Titlebar);
    sf::Event ev{};
	window.setFramerateLimit(144);

	Player test{window};

    while (window.isOpen())
    {
		while (window.pollEvent(ev)) {
			switch (ev.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Escape)
					window.close();
				break;
			}
		}

		window.clear(sf::Color::Black);

		test.updatePlayer();

		test.renderPlayer();

		window.display();
    }

    
}
