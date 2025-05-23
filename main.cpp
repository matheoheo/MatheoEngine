#include "pch.h"
#include "Logger.h"
#include "AssetManager.h"
#include "ConceptDefinitions.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode({800, 600}), "Hi, My Name Is");
	sf::CircleShape circle(100.f);
	ksn::AssetManager<std::string, sf::Texture> textures;
	textures.load("name", "assets/someOther.png");



	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		window.clear();
		window.draw(circle);
		window.display();
	}
}