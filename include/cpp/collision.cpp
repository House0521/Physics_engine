#include "../collision.h"


// collision detection
bool if_collide(Circle& a, Circle& b) {	// circle against circle
	if (sfv::abs(a.get_pos() - b.get_pos()) < a.get_radius() + b.get_radius())
		return true;
	return false;
}

bool if_collide(Polygon& a, Polygon& b) {	//using GJK
	return false;
}


// collision response
void cir_collide(Circle& a, Circle& b) {	// using Hooke's law
	if (!if_collide(a, b))	return;

	long long K = 100;
	float x = (a.get_radius() + b.get_radius()) - sfv::abs(a.get_pos() - b.get_pos());
	float abs_f = K * x;	// F = kx
	a.apply_force(abs_f * sfv::unitv(a.get_pos() - b.get_pos()));
	b.apply_force(abs_f * sfv::unitv(b.get_pos() - a.get_pos()));
}

void collide(Circle& a, Circle& b) {	// circle against circle
	sf::Vector2f normal = b.get_pos() - a.get_pos();// normal : normal vector of the collision, derection : a to b (pos_b-pos_a)
	float e = a.get_e() * b.get_e();
	float j = sfv::dot(-(1.f + e) * (a.get_v() - b.get_v()), normal) / sfv::dot(normal, normal * (1 / a.get_mass() + 1 / b.get_mass()));
	a.set_v(a.get_v() + normal * (j / a.get_mass()));
	b.set_v(b.get_v() - normal * (j / b.get_mass()));
}