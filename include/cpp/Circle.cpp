#include "../Circle.h"
#include "../Shape.h"

//Cons / Decons
Circle::Circle() 
	: _radius(30.f), Shape()
{
	//init vars
	this->_interia = _mass * _radius * _radius / 2;

	//init cir
	this->_cir.setPosition(_pos.x, _pos.y);
	this->_cir.setRadius(_radius);
	this->_cir.setFillColor(_color);

	//set the origin to the center of mass
	this->_cir.setOrigin(sf::Vector2f(_radius, _radius));

}
Circle::Circle(float radius, float mass, float e,  sf::Vector2f pos, sf::Vector2f v, sf::Vector2f a, sf::Color color)
	: _radius(radius), Shape(mass, e, pos, v, a, color, 0, 0)
{
	//init vars
	this->_interia = _mass * _radius * _radius / 2;

	//init cir
	this->_cir.setPosition(_pos.x, _pos.y);
	this->_cir.setRadius(_radius);
	this->_cir.setFillColor(_color);

	//set the origin to the center of the mass
	this->_cir.setOrigin(sf::Vector2f( _radius,_radius));
}

Circle::~Circle() {

}

//setting the properties
void Circle::set_radius(float radius){
	this->_radius = radius;
	this->_cir.setRadius(radius);
	this->_cir.setOrigin(sf::Vector2f(_radius, _radius));
}


//access
sf::CircleShape Circle::body() {
	return this->_cir;
}
float Circle::get_radius() {
	return this->_radius;
}


//draw : update things about the entity
void Circle::update() {
	this->_cir.setPosition(_pos.x, _pos.y);
	this->_cir.setRotation(_theta / PI * 180);	// when there's no friction it's meaningless
	this->_cir.setRadius(_radius);
	this->_cir.setFillColor(_color);
}
