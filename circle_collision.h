#ifndef CIRCLE_COLLISION_H
#define CIRCLE_COLLISION_H

#include "Circle.h"
#include "my_math.h"



bool cir_if_col(Circle& a, Circle& b) {
	if (sfv::abs(a.get_pos() - b.get_pos()) < a.get_radius() + b.get_radius())
		return true;
	return false;
}

void cir_collide(Circle& a, Circle& b) {	// using Hooke's law
	if (!cir_if_col(a, b))	return;	

	long long K = 1000;
	float x = (a.get_radius() + b.get_radius()) - sfv::abs(a.get_pos() - b.get_pos());
	float abs_f = K * x;	// F = kx
	a.apply_force(abs_f * sfv::unitv(a.get_pos() - b.get_pos()));
	b.apply_force(abs_f * sfv::unitv(b.get_pos() - a.get_pos()));
}

#endif // CIRCLE_COLLISION_H