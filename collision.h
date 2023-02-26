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

	long long K = 100;
	float x = (a.get_radius() + b.get_radius()) - sfv::abs(a.get_pos() - b.get_pos());
	float abs_f = K * x;	// F = kx
	a.apply_force(abs_f * sfv::unitv(a.get_pos() - b.get_pos()));
	b.apply_force(abs_f * sfv::unitv(b.get_pos() - a.get_pos()));
}

void collide(Shape& a, Shape & b, sf::Vector2f n) {	// n : normal vector of the collision
	float e = a.get_e() * b.get_e();
	float j = sfv::dot(-(1.f + e) * (a.get_v() - b.get_v()), n) / sfv::dot(n, n * (1 / a.get_mass() + 1 / b.get_mass()));
	a.set_v(a.get_v() + n * (j / a.get_mass()));
	b.set_v(b.get_v() - n * (j / b.get_mass()));
}
#endif // CIRCLE_COLLISION_H