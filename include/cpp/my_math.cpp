#include "../my_math.h"

namespace sfv {
	// Scalar math
	float square(float x) {
		return x * x;
	}

	bool flt_equal(float a, float b) {
		return std::abs(a - b) < EPSILON;
	}


	// Vector math
	float square(const sf::Vector2f& vec) {
		return sfv::dot(vec, vec);
	}

	bool flt_equal(const sf::Vector2f& u, const sf::Vector2f& v) {
		return std::abs(u.x - v.x) < EPSILON && std::abs(u.y - v.y) < EPSILON;
	}

	float det(const sf::Vector2f& u, const sf::Vector2f& v) {
		return u.x * v.y - u.y * v.x;
	}

	float abs(const sf::Vector2f& vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}

	float distance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
		return abs(p1 - p2);
	}

	sf::Vector2f unitv(const sf::Vector2f& vec) {	// return unit vector
		return vec / abs(vec);
	}

	sf::Vector2f normalv(const sf::Vector2f& vec) {	// the abs of normal is the same as vec
		return sf::Vector2f(-vec.y, vec.x);
	}

	float dot(const sf::Vector2f& u, const sf::Vector2f& v) {
		return u.x * v.x + u.y * v.y;
	}

	/*
		Cross product of 2d vectors
		based on https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-oriented-rigid-bodies--gamedev-8032
	*/

	float cross(const sf::Vector2f& u, const sf::Vector2f& v) {	//resembles determinant, giving a scalar
		return u.x * v.y - u.y * v.x;
	}

	sf::Vector2f cross(const sf::Vector2f& v, float s) {
		return sf::Vector2f(s * v.y, -s * v.x);
	}

	sf::Vector2f cross(float s, const sf::Vector2f& v) {
		return sf::Vector2f(-s * v.y, s * v.x);
	}


	sf::Vector2f projection_v(const sf::Vector2f& vec, const sf::Vector2f& axis) {	// 正射影
		return sfv::dot(vec, axis) / sfv::abs(axis) * sfv::unitv(axis);
	}
	float        projection_f(const sf::Vector2f& vec, const sf::Vector2f& axis) {	// 正射影的長度
		return sfv::dot(vec, axis) / sfv::abs(axis);
	}

	float point_lineseg_dis(sf::Vector2f p, sf::Vector2f a, sf::Vector2f b) {	// caculating the distance between a point(p) and a line segment(ab) 
		sf::Vector2f ab = b - a;
		sf::Vector2f pp = a + projection_v(p - a, ab);	// the projection of p
		float ppf = projection_f(p - a, ab);
		if (0 < ppf && ppf < sfv::abs(ab))
			return sfv::distance(p, pp);
		else
			return std::min(sfv::distance(p, a), sfv::distance(p, b));
	}

	//linear transformation
	sf::Vector2f rotate_trans(const sf::Vector2f& v, float theta) {	// using roatation matrix
		return sf::Vector2f(v.x * cos(theta) - v.y * sin(theta), v.x * sin(theta) + v.y * cos(theta));
	}



	//about polygon
	sf::Vector2f com_of_polygon(std::vector<sf::Vector2f> point_pos) {

		/*
			separate the polygon into multiple triangles, and caculate the com and area of each of them
			the com of the polygon should be the weighted average of the coms of triangles, using the areas as weights
		*/
		sf::Vector2f sum_v(0.0, 0.0);
		float sum_a = 0.0;
		for (int i = 0; i < point_pos.size() - 2; i++) {
			sf::Vector2f com_of_triangle = (point_pos[0] + point_pos[i + 1] + point_pos[i + 2]) / 3.f;
			float area_of_triangle = det(point_pos[i + 1] - point_pos[0], point_pos[i + 2] - point_pos[0]) / 2.f;

			sum_v += com_of_triangle * area_of_triangle;
			sum_a += area_of_triangle;
		}

		return sum_v / sum_a;
	}


	float cacl_pol_interia(float mass, std::vector<sf::Vector2f> points) {
		float numerator = 0, denominator = 0;
		for (auto iter = points.begin(); iter != points.end(); iter++) {
			auto next = iter;
			if (iter == points.end() - 1)
				next = points.begin();
			else
				next = iter + 1;
			numerator += sfv::cross(*iter, *next) * (sfv::dot(*iter, *iter) + sfv::dot(*iter, *next) + sfv::dot(*next, *next));
			denominator += sfv::cross(*iter, *next);
		}

		return (mass * numerator) / (6 * denominator);
	}


	bool point_in_polygon(sf::Vector2f p, std::vector<sf::Vector2f> vertex) {	
		float last = 0, now = 0;	// using cross product to see if vec(vertex - p) rotate in the same direction
		for (auto iter = vertex.begin(); iter != vertex.end(); iter++) {
			auto next = iter;
			if (iter == vertex.end() - 1)
				next = vertex.begin();
			else
				next = iter + 1;

			sf::Vector2f va = *iter - p, vb = *next - p;
			last = now;
			now = sfv::cross(va, vb);
			if (last == 0)
				continue;
			if ((last) * (now) >= 0)	// both positive or both negative
				continue;
			if (last * now <= 0)
				return false;
		}
		return true;
	}
}

