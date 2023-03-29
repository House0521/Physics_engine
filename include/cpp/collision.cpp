#include "../collision.h"


// collision detection
Collision_data if_collide(Circle& a, Circle& b) {	// circle against circle
	Collision_data cd;

	cd.normal = a.get_pos() - b.get_pos();
	cd.depth = sfv::abs(cd.normal);
	cd.contact_point = (a.get_pos() * b.get_radius() + b.get_pos() * a.get_radius()) / (a.get_radius() + b.get_radius());

	if (sfv::abs(a.get_pos() - b.get_pos()) < a.get_radius() + b.get_radius()) {
		cd.collide = true;
		return cd;
	}
	return cd;
}


Collision_data if_collide(Polygon& a, Polygon& b) {	//using SAT
	
	Collision_data cd;

	// aps : a's points, bps : b's points
	std::vector<sf::Vector2f> aps = a.get_point_pos(), bps = b.get_point_pos();

	for(int cnt = 0; cnt < 2; cnt++) {		

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


			//std::cout << "amax = " << amax << ", amin = " << amin << "\n";
			// checking if is overlaping
			if (amin > bmax || amax < bmin) {	// do not overlap
				cd.collide = false;
				cd.depth = 0.f;
				cd.normal = sf::Vector2f(0, 0);
				return cd;	// not colliding
			}
			else {
				float depth;
				if (amax >= bmax)	depth = bmax - amin;	// a is on the right
				else	depth = amax - bmin;	// a is on the left

				if (depth < cd.depth || cd.depth == 0.f) {	// finding the mininum resolve depth
					cd.depth = depth;
					cd.normal = normal;
				}
			}
		}		
	}
	cd.collide = true;	// overlaping on every axis --> colliding 

	// -------------------caculating contact point, depth, normal-------------------
	// iterate through all the vertices and edges, and find the closest pair

	cd.depth = FLT_MAX;

	for (int cnt = 0; cnt < 2; cnt++) {
		if (cnt == 1)
			std::swap(aps, bps);
		for (auto iter = aps.begin(); iter != aps.end(); iter++) {	// taking edges
			sf::Vector2f start, end;	// (end - start) is the edge
			if (iter != aps.end() - 1)
				end = *(iter + 1), start = *iter;
			else
				end = *aps.begin(), start = *iter;

			for (auto vertex : bps) {
				float depth = sfv::point_lineseg_dis(vertex, start, end);
				if (depth < cd.depth) {
					cd.depth = depth;
					cd.contact_point = vertex;
				}
			}
		}
	}

	// -------------------caculating contact point, depth, normal-------------------

	return cd;
}


Collision_data if_collide(Polygon& p, Circle& c) {	//using SAT
	Collision_data cd;

	// pps : p's points
	const std::vector<sf::Vector2f> &pps = p.get_point_pos();
	// cc : center of circle
	sf::Vector2f cc = c.get_pos();
	
	// taking edges' normal as axis to project on
	for (auto point = pps.begin(); point <= pps.end(); point++) {

		// caculating the normal vector of the edge to project on
		sf::Vector2f normal;
		if (point != pps.end()) {
			if (point != pps.end() - 1)
				normal = sfv::normalv(*next(point) - *point);	// point_next - point_now = vec(edge)
			else
				normal = sfv::normalv(*pps.begin() - *point);	// the next edge of the last edge is the first one
		}
		else {	// point == pps.end() --> already done every edge of the polygon
			float min_dist = FLT_MAX;
			sf::Vector2f np;	
			for (auto p : pps) {	// determine the vertex that is the nearest to the circle
				if (sfv::distance(cc, p) < min_dist)
					np = p;
			}
			normal = np - cc;
		}

		// doing projection
		float pmax = -FLT_MAX, pmin = FLT_MAX, cmax, cmin;	// min/max of the projection of the points of a,b on the axis
		for (auto iter = pps.begin(); iter != pps.end(); iter++) {	//projecting the polygon
			float projection = sfv::projection_f(*iter, normal);
			pmax = std::max(projection, pmax);
			pmin = std::min(projection, pmin);
		}
		float ccp = sfv::projection_f(cc, normal);	// the projection of cc
		cmax = ccp + c.get_radius();
		cmin = ccp - c.get_radius();

		// checking if is overlaping
		if (pmin > cmax || pmax < cmin) {	// do not overlap
			cd.depth = 0;
			cd.normal = sf::Vector2f(0, 0);
			cd.collide = false;
			return cd;	// not colliding
		}
		if (point == pps.end())	// prevent point from iterate over pps.end()
			break;
	}

	// is colliding
	cd.collide = true;

	// -------------------caculating contact point, depth, normal-------------------
	cd.depth = FLT_MAX;

	// when the contact point is on one of the edges of p
	for (auto point = pps.begin(); point != pps.end(); point++) {
		// seeing an edge as a segment of a line
		sf::Vector2f a = *point;	// taking point a, b
		sf::Vector2f b;
		if (point == pps.end() - 1)	
			b = *pps.begin();
		else 
			b = *(point + 1);
		sf::Vector2f ab = b - a;
		
		float ccp = sfv::projection_f(cc - a, ab);	// the projection of cc
		sf::Vector2f ccpv = a + sfv::projection_v(cc - a, ab);
		if (0 < ccp && ccp < sfv::abs(ab) && sfv::abs(ccpv - cc) < c.get_radius()){	// the projevtion of cc is between a and b  &&  d(cc, edge) < r
			float depth = c.get_radius() - sfv::abs(ccpv - cc);
			if (depth < cd.depth) {
				cd.depth = depth;
				cd.contact_point = ccpv;	// contact point found
			}
		}

	}

	// when the contact point is on one of the vertices of p

	for (auto& point : pps) {
		if (sfv::distance(point, cc) > c.get_radius())
			continue;
		float depth = c.get_radius() - sfv::distance(point, cc);
		if (depth < cd.depth) {
			cd.depth = depth;
			cd.contact_point = point;	// contact point found
		}
	}
	
	cd.normal = cd.contact_point - cc;

	// -------------------caculating contact point, depth, normal-------------------

	return cd;
}

Collision_data if_collide(Circle& c, Polygon& p) {
	return if_collide(p, c);
}

// collision response
void cir_collide(Circle& a, Circle& b) {	// using Hooke's law
	if (!if_collide(a, b).collide)	return;

	long long K = 100;
	float x = (a.get_radius() + b.get_radius()) - sfv::abs(a.get_pos() - b.get_pos());
	float abs_f = K * x;	// F = kx
	a.apply_force(abs_f * sfv::unitv(a.get_pos() - b.get_pos()));
	b.apply_force(abs_f * sfv::unitv(b.get_pos() - a.get_pos()));
}

void collide(Circle& a, Circle& b) {	// circle against circle, using impulse method
	sf::Vector2f normal = b.get_pos() - a.get_pos();	// normal : normal vector of the collision, derection : a to b (pos_b-pos_a)
	float e = a.get_e() * b.get_e();	// or min(a.e, b.e)
	float j = sfv::dot(-(1.f + e) * (a.get_v() - b.get_v()), normal) / sfv::dot(normal, normal * (1 / a.get_mass() + 1 / b.get_mass()));
	a.set_v(a.get_v() + normal * (j / a.get_mass()));
	b.set_v(b.get_v() - normal * (j / b.get_mass()));
}