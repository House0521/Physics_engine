#include <iostream>

#include "../Shape.h"
#include "../rk4.h"

//Cons / Decons
Shape::Shape()
	: _mass(1.f), _rec_mass(1 / _mass), _e(0.9), _inertia(0), _pos(sf::Vector2f(0.f, 0.f)), _v(sf::Vector2f(0.f, 0.f)), _a(sf::Vector2f(0.f, 0.f)), _color(sf::Color::Cyan), _theta(0), _omega(0)
{

}
Shape::Shape(float mass, float e, sf::Vector2f pos, sf::Vector2f v, sf::Vector2f a, sf::Color color, float theta, float omega)
	: _mass(mass), _rec_mass(1 / mass), _e(e), _inertia(0), _pos(pos), _v(v), _a(a), _color(color), _theta(theta), _omega(omega)
{

}

Shape::~Shape() {

}

//setting the properties
void Shape::set_pos(sf::Vector2f pos) {
	if (this->_immovable)
		return;
	this->_pos = pos;
}
void Shape::set_pos(float x, float y) {
	if (this->_immovable)
		return;
	this->_pos.x = x, this->_pos.y = y;
}
void Shape::set_v(sf::Vector2f v) {
	if (this->_immovable)
		return;
	this->_v = v;
}
void Shape::set_v(float x, float y) {
	if (this->_immovable)
		return;
	this->_v.x = x, this->_v.y = y;
}
void Shape::set_a(sf::Vector2f a) {
	if (this->_immovable)
		return;
	this->_a = a;
}
void Shape::set_a(float x, float y) {
	if (this->_immovable)
		return;
	this->_a.x = x, this->_a.y = y;
}
void Shape::set_theta(float theta) {
	if (this->_immovable)
		return;
	this->_theta = theta;
}
void Shape::set_omega(float omega) {
	if (this->_immovable)
		return;
	this->_omega = omega;
}
void Shape::set_color(sf::Color color) {
	_color = color;
}
void Shape::set_e(float e) {
	if (this->_immovable)
		return;
	_e = e;
}
void Shape::set_mass(float mass) {
	_mass = mass;
	_rec_mass = 1 / mass;
}
void Shape::set_immovable(bool immovable) {
	_immovable = immovable;
	if (immovable) {
		this->_v = sf::Vector2f(0, 0);
		this->_a = sf::Vector2f(0, 0);
		this->_omega = 0;
	}
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
float Shape::get_theta() {
	return this->_theta;
}
float Shape::get_omega() {
	return this->_omega;
}
sf::Color Shape::get_color() {
	return this->_color;
}
float Shape::get_e() {
	return this->_e;
}
float Shape::get_mass() {
	if (this->_immovable)
		return FLT_MAX;
	return this->_mass;
}
float Shape::get_rec_mass() {
	if (this->_immovable)
		return 0.0f;
	return this->_rec_mass;
}
float Shape::get_inertia() {
	if (this->_immovable)
		return FLT_MAX;
	return this->_inertia;
}
float Shape::get_rec_inertia() {
	if (this->_immovable)
		return 0.0f;
	return 1.f / this->_inertia;
}
bool Shape::get_immovable() {
	return this->_immovable;
}


// move
void Shape::move(sf::Vector2f vec) {
	this->_pos += vec;
}
void Shape::move(float x, float y) {
	this->_pos += sf::Vector2f(x, y);
}
void Shape::rotate(float theta) {
	this->_theta += theta;
}

//interacting
void Shape::apply_force(sf::Vector2f f) {
	if (this->_immovable)
		return;
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
float Shape::theta_prime(float delta_t) {
	return this->_omega;
}

//motion
void Shape::step(float dt) {

	/*
		Eular's method :
			_pos += _v * dt;
			_v += _a * dt;
	*/
	//_pos += _v * dt;
	//_v += _a * dt;
	this->_theta += _omega * dt;

	//Runge Kutta 4th
	this->_pos = rk4<Shape, sf::Vector2f>(this->_pos, dt, this, &Shape::pos_prime);
	this->_v = rk4<Shape, sf::Vector2f>(this->_v, dt, this, &Shape::v_prime);
	//this->_theta = rk4<Shape, float>(this->_theta, dt, this, &Shape::theta_prime);

	
	//refresh_forces();
}

