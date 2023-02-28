#include <iostream>

#include "../Shape.h"
#include "../rk4.h"

//Cons / Decons
Shape::Shape()
	: _mass(1.f), _e(1), _pos(sf::Vector2f(0.f, 0.f)), _v(sf::Vector2f(0.f, 0.f)), _a(sf::Vector2f(0.f, 0.f)), _color(sf::Color::Cyan)
{

}
Shape::Shape(float mass, float e, sf::Vector2f pos, sf::Vector2f v, sf::Vector2f a, sf::Color color)
	: _mass(mass), _e(e), _pos(pos), _v(v), _a(a), _color(color)
{

}

Shape::~Shape() {

}

//setting the properties
void Shape::set_pos(sf::Vector2f pos) {
	this->_pos = pos;
}
void Shape::set_pos(float x, float y) {
	this->_pos.x = x, this->_pos.y = y;
}
void Shape::set_v(sf::Vector2f v) {
	this->_v = v;
}
void Shape::set_v(float x, float y) {
	this->_v.x = x, this->_v.y = y;
}
void Shape::set_a(sf::Vector2f a) {
	this->_a = a;
}
void Shape::set_a(float x, float y) {
	this->_a.x = x, this->_a.y = y;
}
void Shape::set_color(sf::Color color) {
	_color = color;
}

//access
sf::Vector2f Shape::get_pos() {
	return this->_pos;
}
sf::Vector2f Shape::get_v() {
	return this->_v;
}
sf::Vector2f Shape::get_a() {
	return this->_a;
}
/*
sf::Vector2f Shape::get_com() {
	return this->_com;
}
*/
sf::Color Shape::get_color() {
	return this->_color;
}
float Shape::get_e() {
	return this->_e;
}
float Shape::get_mass() {
	return this->_mass;
}

//interacting
void Shape::apply_force(sf::Vector2f f) {
	this->_a += f / _mass;	// F = ma
}
void Shape::refresh_forces() {
	this->_a = sf::Vector2f(0, 0);
}


//for rk4
sf::Vector2f Shape::v_prime(float delta_t) {	//v' = a
	return this->_a;
}
sf::Vector2f Shape::pos_prime(float delta_t) {	//pos' = v
	return this->_v + this->_a * delta_t;
}

//motion
void Shape::iterate(float dt) {
	/*
		Eular's method :
			_pos += _v * dt;
			_v += _a * dt;
	*/
	//_pos += _v * dt;
	//_v += _a * dt;

	//Runge Kutta 4th
	this->_pos = rk4<Shape, sf::Vector2f>(this->_pos, dt, this, &Shape::pos_prime);
	this->_v = rk4<Shape, sf::Vector2f>(this->_v, dt, this, &Shape::v_prime);

	
	//refresh_forces();
}

