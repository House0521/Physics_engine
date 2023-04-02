#ifndef COLLISION_H
#define COLLISION_H

#include "Circle.h"
#include "Polygon.h"
#include "my_math.h"
#include <iostream>

#define INIT_VEC sf::Vector2f(FLT_MAX, FLT_MAX)
#define INIT_SCA FLT_MAX

struct Collision_data {
	bool collide = false;	// collide or not
	int contact_point_count = 1;
	std::vector<sf::Vector2f> normal = { INIT_VEC, INIT_VEC };
	std::vector<sf::Vector2f> contact_points = { INIT_VEC, INIT_VEC };
	float depth = INIT_SCA;
};

// collision detection
Collision_data if_collide(Circle& a, Circle& b);	// circle against circle
Collision_data if_collide(Polygon& a, Polygon& b);	//using SAT
Collision_data if_collide(Polygon& p, Circle& c);
Collision_data if_collide(Circle& c, Polygon& p);

// collision response
void cir_collide(Circle& a, Circle& b);	// only for circle, using spring method

//void collide(Circle& a, Circle& b);	// using impulse method
//void collide(Polygon& a, Polygon& b, Collision_data cd);	// using impulse method, rotation included
//void collide(Polygon& p, Circle& c, Collision_data cd);
//void collide(Circle& c, Polygon& p, Collision_data cd);
void collide(Shape& a, Shape& b, Collision_data cd);


#endif // COLLISION_H