#include "../Polygon.h"
#include "../rk4.h"
#include "../my_math.h"

Polygon::Polygon(std::vector<sf::Vector2f> point_pos)
	: _point_count(point_pos.size()), _point_pos(point_pos), Shape()
{
	// setting origin
	this->_polygon.setOrigin(sfv::com_of_polygon(_point_pos));

	//init polyogn object
	_polygon.setPointCount(_point_pos.size());
	for (int i = 0; i < _point_pos.size(); i++) {
		_polygon.setPoint(i, _point_pos[i]);
	}
}

Polygon::~Polygon() {
	
}

//access
sf::ConvexShape Polygon::body() {
	return _polygon;
}

int Polygon::get_point_count() {
	return this->_point_count;
}

const std::vector<sf::Vector2f>& Polygon::get_point_pos() {
	return this->_point_pos;
}

//draw
void Polygon::draw() {
	_polygon.setPosition(_pos);
	_polygon.setFillColor(_color);
	//_polygon.setRotation();

}