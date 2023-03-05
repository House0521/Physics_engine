/*
	only for convex polygons
*/

#ifndef POLYGON_H
#define POLYGON_H

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Shape.h"

class Polygon : public Shape {
private:
	int _point_count;
	std::vector<sf::Vector2f> _rel_point_pos;	//points' positions relative to the com
	sf::ConvexShape _polygon;

public:
	Polygon(std::vector<sf::Vector2f> point_pos);	// the positions have to be ordered
	~Polygon();

	//access
	sf::ConvexShape body();
	int get_point_count();
	std::vector<sf::Vector2f> get_point_pos();

	//draw
	void update();

};

#endif // POLYGON_H