#include "Circle.h"
#include "rk4.h"

//Cons / Decons
Circle::Circle() 
	: _radius(30.f), Shape()
{
	//init cir
	this->_cir.setPosition(_pos.x, _pos.y);
	this->_cir.setRadius(_radius);
	this->_cir.setFillColor(_color);

	//caculate com
	this->_com = sf::Vector2f(_pos.x + _radius, _pos.y + _radius);
}
Circle::Circle(float radius, float mass, float e,  sf::Vector2f pos, sf::Vector2f v, sf::Vector2f a, sf::Color color)
	: _radius(radius), Shape(mass, e, pos, v, a, color)
{
	//init cir
	this->_cir.setPosition(_pos.x, _pos.y);
	this->_cir.setRadius(_radius);
	this->_cir.setFillColor(_color);

	//caculate com
	this->_com = sf::Vector2f(_pos.x + _radius, _pos.y + _radius);
}

Circle::~Circle() {

}

//setting the properties
void Circle::set_radius(float radius){
	this->_radius = radius;
	this->_cir.setRadius(radius);
}


//access
sf::CircleShape Circle::body() {
	return this->_cir;
}
float Circle::get_radius() {
	return this->_radius;
}


//draw
void Circle::draw() {
	this->_cir.setPosition(_pos.x, _pos.y);
	this->_cir.setRadius(_radius);
	this->_cir.setFillColor(_color);
}
