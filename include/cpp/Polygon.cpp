#include "../Polygon.h"
#include "../rk4.h"
#include "../my_math.h"

Polygon::Polygon(std::vector<sf::Vector2f> point_pos)
	: _point_count(point_pos.size()), _rel_point_pos(point_pos), Shape()
{
	// init vars
	this->_pos = sfv::com_of_polygon(point_pos);

	for (auto& point : _rel_point_pos) {
		point -= _pos;
	}

	this->_inertia = sfv::cacl_pol_interia(_mass, point_pos);


	// init polyogn object
	_polygon.setPointCount(point_pos.size());
	for (auto iter = point_pos.begin(); iter != point_pos.end(); iter++) {
		_polygon.setPoint(iter - point_pos.begin(), *iter);
		
	}

	// setting origin
	// have to do like this because sfml origin is setted relatively to the top left of the entity
	this->_polygon.setOrigin(sfv::com_of_polygon(point_pos) - _polygon.getPosition());
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

const std::vector<sf::Vector2f> Polygon::get_point_pos() {
	std::vector<sf::Vector2f> real_point_pos;
	for (auto point : this->_rel_point_pos) {
		real_point_pos.push_back(sfv::rotate_trans(point, this->_theta));
	}
	for (auto& point : real_point_pos) {
		point += this->_pos;
	}
	return real_point_pos;
}

//draw
void Polygon::update() {
	_polygon.setPosition(_pos);
	_polygon.setRotation(_theta / PI * 180);
	_polygon.setFillColor(_color);
	//_polygon.setRotation();

}