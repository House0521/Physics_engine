#ifndef MY_MATH_H
#define MY_MATH_H

#include <cmath>
#include <vector>
#include <SFML/System/Vector2.hpp>


namespace sfv {
	//Vector math
	float det(const sf::Vector2f& u, const sf::Vector2f& v);
	float abs(const sf::Vector2f& vec);
	float distance(const sf::Vector2f& p1, const sf::Vector2f& p2);
	sf::Vector2f unitv(const sf::Vector2f& vec);
	sf::Vector2f normalv(const sf::Vector2f& vec);
	float dot(const sf::Vector2f& u, const sf::Vector2f& v);
	float cross(const sf::Vector2f& u, const sf::Vector2f& v);
	sf::Vector2f cross(const sf::Vector2f& v, float s);
	sf::Vector2f cross(float s, const sf::Vector2f& v);

	sf::Vector2f projection_v(const sf::Vector2f& vec, const sf::Vector2f& axis);	// project vec on axis, returning the vector
	float        projection_f(const sf::Vector2f& vec, const sf::Vector2f& axis);	// project vec on axis, returning the length

	float point_lineseg_dis(sf::Vector2f p, sf::Vector2f a, sf::Vector2f b);

	//linear trandformation
	sf::Vector2f rotate_trans(const sf::Vector2f& v, float theta);

	//about polygon
	sf::Vector2f com_of_polygon(std::vector<sf::Vector2f> point_pos);
	float cacl_pol_interia(float mass, std::vector<sf::Vector2f> points);
}

#endif // MY_MATH_H