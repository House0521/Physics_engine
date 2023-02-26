#ifndef SHAPE_H
#define SHAPE_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Shape {
protected:
	float _mass, _e;	// e : coefficient of restitution
	sf::Vector2f _pos, _v, _a, _com;	// com : center of mass
	sf::Color _color;

public:
	Shape();
	Shape(float mass, float e, sf::Vector2f pos, sf::Vector2f v, sf::Vector2f a, sf::Color color);
	~Shape();


	//setting the properties
	void set_pos(sf::Vector2f pos);
	void set_pos(float x, float y);
	void set_v(sf::Vector2f v);
	void set_v(float x, float y);
	void set_a(sf::Vector2f a);
	void set_a(float x, float y);
	void set_color(sf::Color color);

	//access
	sf::Vector2f get_pos();
	sf::Vector2f get_v();
	sf::Vector2f get_a();
	sf::Vector2f get_com();
	sf::Color get_color();
	float get_e();
	float get_mass();

	//interacting
	void apply_force(sf::Vector2f f);
	void refresh_forces();	// prevent temporary forces from making impact permanently

	//for rk4
	sf::Vector2f v_prime(float delta_t);	//v' = a
	sf::Vector2f pos_prime(float delta_t);	//pos' = v

	//motion
	void iterate(float dt);



};

#endif // SHAPE_H
