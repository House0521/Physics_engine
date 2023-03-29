#ifndef COLLISION_H
#define COLLISION_H

#include "Circle.h"
#include "Polygon.h"
#include "my_math.h"
#include <iostream>

struct Collision_data {
	bool collide = false;	// collide or not
	sf::Vector2f normal;
	sf::Vector2f contact_point;
	float depth = 0.f;
};

// collision detection
Collision_data if_collide(Circle& a, Circle& b);	// circle against circle
Collision_data if_collide(Polygon& a, Polygon& b);	//using SAT
Collision_data if_collide(Polygon& p, Circle& c);
Collision_data if_collide(Circle& c, Polygon& p);

// collision response
void cir_collide(Circle& a, Circle& b);	// only for circle, using spring method
void collide(Circle& a, Circle& b);	// using inpulse method


#endif // COLLISION_H