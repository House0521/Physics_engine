#ifndef CIRCLE_H
#define CIRCLE_H

#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "Shape.h"

#define PI 3.14159265

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
	void update();

};

#endif // CIRCLE_H
