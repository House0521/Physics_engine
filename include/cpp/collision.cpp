#include "../collision.h"


// collision detection
Collision_data if_collide(Circle& a, Circle& b) {	// circle against circle
	Collision_data cd;

	cd.normal[0] = b.get_pos() - a.get_pos();
	cd.depth = a.get_radius() + b.get_radius() - sfv::abs(cd.normal[0]);
	cd.contact_points[0] = (a.get_pos() * b.get_radius() + b.get_pos() * a.get_radius()) / (a.get_radius() + b.get_radius());

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
				cd.depth = INIT_SCA;
				cd.normal[0] = INIT_VEC;
				return cd;	// not colliding
			}
		}		
	}
	cd.collide = true;	// overlaping on every axis --> colliding 

	// -------------------caculating contact point, depth, normal : start-------------------
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
				if (!sfv::point_in_polygon(vertex, aps)) {
					//std::cout << "skipping vertex : (" << vertex.x << ", " << vertex.y << ")\n";
					//system("pause");
					continue;
				}
				float depth = sfv::point_lineseg_dis(vertex, start, end);
				if (sfv::flt_equal(depth, cd.depth)) {
					if (sfv::flt_equal(vertex, cd.contact_points[0]))	
						continue;
					cd.contact_point_count = 2;
					cd.contact_points[1] = vertex;
					cd.normal[1] = sfv::normalv(end - start);
				}
				else if (depth < cd.depth) {
					cd.depth = depth;
					//std::cout << "setting depth : " << depth << ", ";
					//std::cout << "at point " << "(" << vertex.x << ", " << vertex.y << ")\n";
					//system("pause");
					cd.contact_points[0] = vertex;
					cd.contact_point_count = 1;
					cd.normal[0] = sfv::normalv(end - start);
				}
			}
		}
	}


	// -------------------caculating contact point, depth, normal : end-------------------


	// last check
	if (cd.contact_points[0] == INIT_VEC && cd.depth == INIT_SCA)
		cd.collide = false;

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
			cd.depth = INIT_SCA;
			cd.normal[0] = INIT_VEC;
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
			float depth = c.get_radius() - sfv::point_lineseg_dis(cc, a, b);
			if (depth < cd.depth) {
				cd.depth = depth;
				cd.contact_points[0] = ccpv;	// contact point found
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
			cd.contact_points[0] = point;	// contact point found
		}
	}
	
	if (cd.depth == FLT_MAX || cd.contact_points[0] == INIT_VEC) {
		cd.normal[0] = INIT_VEC;
		cd.collide = false;
		return cd;	// not colliding
	}
	cd.normal[0] = cd.contact_points[0] - cc;

	// -------------------caculating contact point, depth, normal-------------------

	return cd;
}

Collision_data if_collide(Circle& c, Polygon& p) {
	return if_collide(p, c);
}


// collision response

// old function, not used anymore
/*
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
	float j = sfv::dot(-(1.f + e) * (a.get_v() - b.get_v()), normal) / sfv::dot(normal, normal * (a.get_rec_mass() + b.get_rec_mass()));
	a.set_v(a.get_v() + normal * (j / a.get_mass()));
	b.set_v(b.get_v() - normal * (j / b.get_mass()));
}

void collide(Polygon& a, Polygon& b, Collision_data cd) {
	if (!cd.collide)
		return;

	float e = a.get_e() * b.get_e();	// or min(a.e, b.e)
	sf::Vector2f n = cd.normal;
	sf::Vector2f ra = cd.contact_point - a.get_pos();
	sf::Vector2f rb = cd.contact_point - b.get_pos();
	float Ia = a.get_inertia();
	float Ib = b.get_inertia();

	float j_numerator = -(1.f + e) * sfv::dot((a.get_v() - b.get_v()), n);
	float j_denominator = sfv::dot(n, n * (a.get_rec_mass() + b.get_rec_mass())) + sfv::square(sfv::dot(ra, n)) / Ia + sfv::square(sfv::dot(rb, n)) / Ib;
	float j = j_numerator / j_denominator;

	a.set_v(a.get_v() + n * (j / a.get_mass()));
	a.set_omega(a.get_omega() + sfv::dot(ra, j * n) / Ia);
	b.set_v(b.get_v() - n * (j / b.get_mass()));
	b.set_omega(a.get_omega() - sfv::dot(rb, j * n) / Ib);
}

void collide(Polygon& p, Circle& c, Collision_data cd) {
	if (!cd.collide)
		return;

	float e = p.get_e() * c.get_e();	// or min(a.e, b.e)
	sf::Vector2f n = cd.normal;
	sf::Vector2f ra = cd.contact_point - p.get_pos();
	sf::Vector2f rb = cd.contact_point - c.get_pos();
	float Ip = p.get_inertia();
	float Ic = c.get_inertia();

	float j_numerator = -(1.f + e) * sfv::dot((p.get_v() - c.get_v()), n);
	float j_denominator = sfv::dot(n, n * (p.get_rec_mass() + c.get_rec_mass())) + sfv::square(sfv::dot(ra, n)) / Ip + sfv::square(sfv::dot(rb, n)) / Ic;
	float j = j_numerator / j_denominator;

	p.set_v(p.get_v() + n * (j / p.get_mass()));
	p.set_omega(p.get_omega() + sfv::dot(ra, j * n) / Ip);
	c.set_v(c.get_v() - n * (j / c.get_mass()));
	c.set_omega(c.get_omega() - sfv::dot(rb, j * n) / Ic);
}

void collide(Circle& c, Polygon& p, Collision_data cd) {
	collide(p, c, cd);
}
*/

void collide(Shape& a, Shape& b, Collision_data cd) {
	if (!cd.collide)
		return;

	// delta vars
	sf::Vector2f del_va = sf::Vector2f(0, 0), del_vb = sf::Vector2f(0, 0);
	float del_omega_a = 0, del_omega_b = 0;


	sf::Vector2f n = cd.normal[0];
	for (int i = 0; i < cd.contact_point_count; i++) {

		float e = a.get_e() * b.get_e();	// or min(a.e, b.e)

		sf::Vector2f ra = sfv::normalv(cd.contact_points[i] - a.get_pos());
		sf::Vector2f rb = sfv::normalv(cd.contact_points[i] - b.get_pos());
		sf::Vector2f vap = a.get_v() + a.get_omega() * ra;
		sf::Vector2f vbp = b.get_v() + b.get_omega() * rb;
		float rec_Ia = a.get_rec_inertia();
		float rec_Ib = b.get_rec_inertia();

		float j_numerator = -(1.f + e) * sfv::dot((vap - vbp), n);
		float j_denominator = sfv::dot(n, n * (a.get_rec_mass() + b.get_rec_mass())) + sfv::square(sfv::dot(ra, n)) * rec_Ia + sfv::square(sfv::dot(rb, n)) * rec_Ib;
		float j = j_numerator / j_denominator;

		j /= cd.contact_point_count;
		
		del_va += n * (j * a.get_rec_mass());
		del_omega_a +=  sfv::dot(ra, j * n) * rec_Ia;
		del_vb += n * (-j * b.get_rec_mass());
		del_omega_b += sfv::dot(rb, -j * n) * rec_Ib;
	}

	a.set_v(a.get_v() + del_va);
	a.set_omega(a.get_omega() + del_omega_a);
	b.set_v(b.get_v() + del_vb);
	b.set_omega(b.get_omega() + del_omega_b);

	sf::Vector2f overlap = sfv::unitv(n) * cd.depth * (float)0.9;
	if (sfv::dot(overlap, a.get_pos() - b.get_pos()) < 0)
		overlap = -overlap;

	
	if (a.get_immovable())
		b.move(-overlap);
	else if (b.get_immovable())
		a.move( overlap);
	else {
		a.move( overlap * b.get_mass() / (a.get_mass() + b.get_mass()));
		b.move(-overlap * a.get_mass() / (a.get_mass() + b.get_mass()));
	}
	

	//std::cout << "del_va = (" << del_va.x << ", " << del_va.y << ")\n";
	//std::cout << "del_omega_a = " << del_omega_a << "\n";
	//system("pause");

}