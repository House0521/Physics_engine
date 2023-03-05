#include "../collision.h"


// collision detection
bool if_collide(Circle& a, Circle& b) {	// circle against circle
	if (sfv::abs(a.get_pos() - b.get_pos()) < a.get_radius() + b.get_radius())
		return true;
	return false;
}

bool if_collide(Polygon& a, Polygon& b) {	//using SAT
	
	// aps : a's points, bps : b's points
	std::vector<sf::Vector2f> aps = a.get_point_pos(), bps = b.get_point_pos();

	int cnt = 0;
	while (cnt < 2) {		

		//taking one polygon to make axises
		std::vector<sf::Vector2f> taking;
		if (cnt == 0)
			taking = aps;
		else
			taking = bps;

		// taking edges' normal as axis to project on
		for (auto point = taking.begin(); point != taking.end(); point++) {
			// caculating the normal vector of the edge to project on
			sf::Vector2f normal;

			if (point != taking.end() - 1)
				normal = sfv::normalv(*next(point) - *point);	// point_next - point_now = vec(edge)
			else
				normal = sfv::normalv(*taking.begin() - *point);	// the next edge of the last edge is the first one

			// doing projection
			float amax = -FLT_MAX, amin = FLT_MAX, bmax = -FLT_MAX, bmin = FLT_MAX;	// of the projection of the points of a,b on the axis
			for (auto a_point = aps.begin(); a_point != aps.end(); a_point++) {	//projecting a's points
				float projection = sfv::projection_f(*a_point, normal);
				amax = std::max(projection, amax);
				amin = std::min(projection, amin);
			}
			for (auto b_point = bps.begin(); b_point != bps.end(); b_point++) {	//projecting b's points
				float projection = sfv::projection_f(*b_point, normal);
				bmax = std::max(projection, bmax);
				bmin = std::min(projection, bmin);
			}

			// checking if is overlaping
			if (amin > bmax || amax < bmin)	// do not overlap
				return false;
			else
				continue;
		}
		cnt++;
	}

	return true;
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