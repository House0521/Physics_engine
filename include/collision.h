#ifndef COLLISION_H
#define COLLISION_H

#include "Circle.h"
#include "Polygon.h"
#include "my_math.h"

// collision detection
bool if_collide(Circle& a, Circle& b);	// circle against circle
bool if_collide(Polygon& a, Polygon& b);	//using SAT

// collision response
void cir_collide(Circle& a, Circle& b);	// only for circle, using spring method
void collide(Circle& a, Circle& b);	// using inpulse method


#endif // COLLISION_H