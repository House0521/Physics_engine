#ifndef CIRCLE_H
#define CIRCLE_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "Shape.h"


class Circle : public Shape{
private:
	sf::CircleShape _cir;
	float _radius;
	

public:
	Circle();
	Circle(float radius, float mass, float e, sf::Vector2f pos, sf::Vector2f v, sf::Vector2f a, sf::Color color);
	~Circle();


	//setting the properties
	void set_radius(float raduis);

	//access
	sf::CircleShape body();
	float get_radius();

	//draw
	void draw();

};

#endif // CIRCLE_H
