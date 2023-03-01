#include "../my_math.h"

namespace sfv {

	// Vector math
	float det(const sf::Vector2f& u, const sf::Vector2f& v) {
		return u.x * v.y - u.y * v.x;
	}

	float abs(const sf::Vector2f& vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}

	sf::Vector2f unitv(const sf::Vector2f& vec) {	// return unit vector
		return vec / abs(vec);
	}

	sf::Vector2f normalv(const sf::Vector2f& vec) {	// the abs of normal is the same as vec
		return sf::Vector2f(-vec.x, vec.y);
	}

	float dot(const sf::Vector2f& u, const sf::Vector2f& v) {
		return u.x * v.x + u.y * v.y;
	}

	/*
		Cross product of 2d vectors
		based on https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-oriented-rigid-bodies--gamedev-8032
	*/

	float cross(const sf::Vector2f& u, const sf::Vector2f& v) {	//resembles determinant
		return u.x * v.y - u.y * v.x;
	}

	sf::Vector2f cross(const sf::Vector2f& v, float s) {
		return sf::Vector2f(s * v.y, -s * v.x);
	}

	sf::Vector2f cross(float s, const sf::Vector2f& v) {
		return sf::Vector2f(-s * v.y, s * v.x);
	}

	sf::Vector2f projection_v(const sf::Vector2f& vec, const sf::Vector2f& axis) {	// project vec on axis, returning the vector
		return sfv::dot(vec, axis) * sfv::unitv(axis);
	}
	float        projection_f(const sf::Vector2f& vec, const sf::Vector2f& axis) {	// project vec on axis, returning the length
		return sfv::dot(vec, axis) / sfv::abs(axis);
	}


	//linear transformation
	sf::Vector2f rotate(const sf::Vector2f& v, float theta) {	// using roatation matrix
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
}