#include "../collision.h"


// collision detection
bool if_collide(Circle& a, Circle& b) {	// circle against circle
	if (sfv::abs(a.get_pos() - b.get_pos()) < a.get_radius() + b.get_radius())
		return true;
	return false;
}

bool if_collide(Polygon& a, Polygon& b) {	//using SAT
	std::vector<sf::Vector2f> ap = a.get_point_pos(), bp = b.get_point_pos();

	int cnt = 0;
	while (cnt < 2) {
		// taking a's edges' normal as axis to project on
		for (int i = 0; i < ap.size(); i++) {
			// caculating normal
			sf::Vector2f normal;
			if (i != ap.size() - 1)
				normal = sfv::normalv(sf::Vector2f(ap[i + 1].x - ap[i].x, ap[i + 1].y - ap[i].y));
			else
				normal = sfv::normalv(sf::Vector2f(ap[0].x - ap[i].x, ap[0].y - ap[i].y));	// the next edge of the last edge is the first one

			// doing projection
			float amax = -FLT_MAX, amin = FLT_MAX, bmax = -FLT_MAX, bmin = FLT_MAX;	// of the projection of the points of a,b on the axis
			for (int j = 0; j < ap.size(); j++) {	//projecting a's points
				float projection = sfv::projection_f(ap[i], normal);
				amax = std::max(projection, amax);
				amin = std::min(projection, amin);
			}
			for (int j = 0; j < bp.size(); j++) {	//projecting b's points
				float projection = sfv::projection_f(bp[i], normal);
				bmax = std::max(projection, bmax);
				bmin = std::min(projection, bmin);
			}

			// checking if is overlaping
			if (amax < bmin || amin > bmax) {
				std::swap(ap, bp);
				cnt++;
			}
			else	return true;
		}
	}

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
	sf::Vector2f normal = b.get_pos() - a.get_pos();	// normal : normal vector of the collision, derection : a to b (pos_b-pos_a)
	float e = a.get_e() * b.get_e();	// or min(a.e, b.e)
	float j = sfv::dot(-(1.f + e) * (a.get_v() - b.get_v()), normal) / sfv::dot(normal, normal * (1 / a.get_mass() + 1 / b.get_mass()));
	a.set_v(a.get_v() + normal * (j / a.get_mass()));
	b.set_v(b.get_v() - normal * (j / b.get_mass()));
}