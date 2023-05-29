#include "raycast.h"

sf::Vector2f mouseposition = sf::Vector2f(0, 0);

const int resolution = 256;
const int scale_factor = 2;

const float maxDistance = 100;

sf::Vector2f origin = sf::Vector2f(0,0);

int main()
{
	sf::RenderWindow window(sf::VideoMode(resolution*scale_factor, resolution*scale_factor), "Raycast Lighting");

	float grid[resolution][resolution];

	// set up walls

	wallHandler wHandler;

	// Placeholder walls

	wHandler.addWall(Point(20,10), Point(30, 30));
	wHandler.addWall(Point(200, 100), Point(240, 150));
	wHandler.addWall(Point(120, 60), Point(150, 190));
	wHandler.addWall(Point(20, 150), Point(40, 200));


	window.setFramerateLimit(144);


	bool update = true;
	
	while (window.isOpen())

	{

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		{
			mouseposition = (sf::Vector2f) sf::Mouse::getPosition(window);

			origin.x = mouseposition.x/scale_factor;
			origin.y = mouseposition.y/scale_factor;

			update = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { origin.x -= 5; update = true; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { origin.x += 5; update = true; }

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { origin.y -= 5; update = true; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { origin.y += 5; update = true; }


		if (update == true)
		{

			for (int i = 0; i < resolution; i++)
			{
				for (int j = 0; j < resolution; j++)
				{
					// check if index is blocked

					if (wHandler.inWay(origin, sf::Vector2f(i,j)))
					{
						grid[i][j] = 1;


					} else
					{
						grid[i][j] = 1;

						float distance = sqrt(abs(origin.x-i)*abs(origin.x-i) + abs(origin.y-j)*abs(origin.y-j));

						if (distance < maxDistance)
							grid[i][j] *= std::fmax(0.f,(distance/maxDistance));
						else if (distance < (maxDistance / 4))
							grid[i][j] = 0;
					}


				}
			}

			window.clear(sf::Color::White);
			update = false;

			// Display Grid
			for (int i = 0; i < resolution; i++)
			{
				for (int j = 0; j < resolution; j++)
				{
					sf::RectangleShape shape;

					shape.setSize(sf::Vector2f(scale_factor, scale_factor));
					shape.setPosition(sf::Vector2f(i*scale_factor, j*scale_factor));


					shape.setFillColor(sf::Color(0,0,0, 255*grid[i][j]));

					window.draw(shape);
				}
			}

			// Display Walls

			std::vector<wall> walls = wHandler.getWalls();

			for (int i = 0; i < walls.size(); i++)
			{
				wall currWall = walls[i];

				sf::RectangleShape shape;
				shape.setSize(sf::Vector2f((currWall.end.x-currWall.start.x)*scale_factor, (currWall.end.y-currWall.start.y)*scale_factor));
				shape.setPosition(sf::Vector2f(currWall.start.x*scale_factor, currWall.start.y*scale_factor));
				shape.setFillColor(sf::Color(0,0,0));

				window.draw(shape);
			}

			sf::CircleShape originC;
			originC.setOrigin(5,5);
			originC.setRadius(5);
			originC.setPosition(origin.x*scale_factor,origin.y*scale_factor);
			originC.setFillColor(sf::Color::Red);

			window.draw(originC);

			window.display();
		}

		

	}


	return 0;
}
