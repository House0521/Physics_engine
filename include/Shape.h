#ifndef SHAPE_H
#define SHAPE_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Shape {
protected:
	float _mass, _e, _rec_mass;	// e : coefficient of restitution, rec_mass : the reciprocal of mass
	bool _immovable = false;	// mass = infinity, rec_mass = 0, v.a.omega.force = 0
	sf::Vector2f _pos, _v, _a;
	float _theta, _omega;	// positive value = clockwise, using rad
	sf::Color _color;

public:
	Shape();
	Shape(float mass, float e, sf::Vector2f pos, sf::Vector2f v, sf::Vector2f a, sf::Color color, float rotation, float omega);
	~Shape();


	//setting the properties
	void set_pos(sf::Vector2f pos);
	void set_pos(float x, float y);
	void set_v(sf::Vector2f v);
	void set_v(float x, float y);
	void set_a(sf::Vector2f a);
	void set_a(float x, float y);
	void set_theta(float theta);
	void set_omega(float omega);
	void set_color(sf::Color color);
	void set_e(float e);
	void set_mass(float mass);
	void set_immovable(bool immovable);

	//access
	sf::Vector2f get_pos();
	sf::Vector2f get_v();
	sf::Vector2f get_a();
	float get_theta();
	float get_omega();
	sf::Color get_color();
	float get_e();
	float get_mass();
	float get_rec_mass();
	bool get_immovable();

	// move
	void move(sf::Vector2f vec);
	void move(float x, float y);
	void rotate(float theta);

	//interacting
	void apply_force(sf::Vector2f f);
	void refresh_forces();	// prevent temporary forces from making impact permanently

	//for rk4
	sf::Vector2f v_prime(float delta_t);	//v' = a
	sf::Vector2f pos_prime(float delta_t);	//pos' = v
	float theta_prime(float delta_t);

	//motion
	void step(float dt);



};

#endif // SHAPE_H
