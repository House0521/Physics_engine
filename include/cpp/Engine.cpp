#include <iostream>
#include <string>

#include "../Engine.h"
#include "../collision.h"
#include "../my_math.h"

//Pricate finctions
void Engine::init_vars() {
	this->_window = nullptr;

	this->_framerate = 300;
	this->_dt = float(1) / _framerate;
	this->_timer = 0;
	this->_pause_time = 10;

	this->_gravity = true;
	this->_pause = false;
}

void Engine::init_window() {
	this->_videoMode.height = _win_height;
	this->_videoMode.width = _win_width;

	this->_window = new sf::RenderWindow(this->_videoMode, "Test", sf::Style::Titlebar | sf::Style::Close);
	this->_window->setFramerateLimit(_framerate);
}

void Engine::init_objs() {
	// the floor (pol[0, 1, 2])
	
	std::vector<sf::Vector2f> floor_points;
	floor_points.push_back(sf::Vector2f(0         , _win_height - 10));
	floor_points.push_back(sf::Vector2f(_win_width, _win_height - 10));
	floor_points.push_back(sf::Vector2f(_win_width, _win_height + 50));
	floor_points.push_back(sf::Vector2f(0         , _win_height + 50));
	Polygon floor(floor_points);
	floor.set_immovable(true);
	pols.push_back(floor);
	
	
	std::vector<sf::Vector2f> left_wall_points;
	left_wall_points.push_back(sf::Vector2f(-10, 0));
	left_wall_points.push_back(sf::Vector2f(10, 0));
	left_wall_points.push_back(sf::Vector2f(10, _win_height));
	left_wall_points.push_back(sf::Vector2f(-10, _win_height));
	Polygon left_wall(left_wall_points);
	left_wall.set_immovable(true);
	pols.push_back(left_wall);
	
	std::vector<sf::Vector2f> right_wall_points;
	right_wall_points.push_back(sf::Vector2f(_win_width - 10, 0));
	right_wall_points.push_back(sf::Vector2f(_win_width + 50, 0));
	right_wall_points.push_back(sf::Vector2f(_win_width + 50, _win_height));
	right_wall_points.push_back(sf::Vector2f(_win_width - 10, _win_height));
	Polygon right_wall(right_wall_points);
	right_wall.set_immovable(true);
	pols.push_back(right_wall);

	std::vector<sf::Vector2f> ceiling_points;
	ceiling_points.push_back(sf::Vector2f(0, 0));
	ceiling_points.push_back(sf::Vector2f(_win_width, 0));
	ceiling_points.push_back(sf::Vector2f(_win_width, 10));
	ceiling_points.push_back(sf::Vector2f(0, 10));
	Polygon ceiling(ceiling_points);
	ceiling.set_immovable(true);
	pols.push_back(ceiling);
	
	
	// circles
	
	cirs.push_back(Circle());
	cirs[0].set_radius(30.f);
	cirs[0].set_pos(sf::Vector2f(50.f, 100.f));
	cirs[0].set_v(sf::Vector2f(1000, 100));
	

	
	cirs.push_back(Circle());
	cirs[1].set_radius(50.f);
	cirs[1].set_pos(sf::Vector2f(500.f, 90.f));
	cirs[1].set_mass(25 / 9);
	
	

	// polygons
	
	std::vector<sf::Vector2f> points;
	points.push_back(sf::Vector2f(0.0, 0.0));
	points.push_back(sf::Vector2f(100.0, 0.0));
	points.push_back(sf::Vector2f(100.0, 100.0));
	points.push_back(sf::Vector2f(0.0, 100.0));
	Polygon square(points);
	square.rotate(PI / 3);
	square.set_pos(300, 300);
	square.set_v(0, 0);
	square.set_mass(3.5367795);
	pols.push_back(square);
	
	square.set_pos(400, 100);
	pols.push_back(square);
	
	
}

void Engine::init_texts() {
	if (!this->font.loadFromFile("./fonts/RobotoMono-Regular.ttf")) {
		printf("load font failed\n");
		system("pause");
	}

	this->cir_pos_text.setFont(font);
	this->cir_pos_text.setCharacterSize(20);
	cir_pos_text.setFillColor(sf::Color::White);
	cir_pos_text.setPosition(10.f, 500.f);
}

//Constructors / Destructors
Engine::Engine() {
	this->init_vars();
	this->init_window();
	this->init_objs();
	this->init_texts();
}

Engine::~Engine() {
	delete this->_window;
}

//Accessors
const bool Engine::running() const {
	return this->_window->isOpen();
}

//Funcitons
void Engine::poll_events() {
	while (this->_window->pollEvent(this->_ev)) {
		switch (this->_ev.type) {
		case sf::Event::Closed:
			this->_window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->_ev.key.code == sf::Keyboard::Escape)
				this->_window->close();
			break;
		}
	}
	
}

void Engine::update() {

	this->poll_events();

	this->resolve_collision();

	//this->process_text();


	// step
	if (!this->_pause) {
		// gravity
		if (this->_gravity) {
			for (auto& cir : cirs)
				cir.set_a(sf::Vector2f(0, g) + cir.get_a());	// y positive is donwards
			for (auto& pol : pols)
				pol.set_a(sf::Vector2f(0, g) + pol.get_a());
		}

		// objs step
		for (auto &cir : cirs)
			cir.step(_dt);
		for (auto &pol : pols)
			pol.step(_dt);

		// refresh forces
		for (auto& cir : cirs)
			cir.refresh_forces();
		for (auto& pol : pols)
			pol.refresh_forces();

		_timer += _dt;

		//if (_timer >= _pause_time)	_pause = true;
	}
	
	// update objs
	for (auto &cir : cirs)
		cir.update();
	for (auto &pol : pols)
		pol.update();
	for (auto& point : points)
		point.update();
}

void Engine::render() {

	this->_window->clear();

	//draw object
	for (auto &cir : cirs)
		this->_window->draw(cir.body());
	for (auto &pol : pols)
		this->_window->draw(pol.body());
	for (auto& point : points)
		this->_window->draw(point.body());

	//draw text
	this->_window->draw(cir_pos_text);

	// display
	this->_window->display();
}

void Engine::pause(bool pau) {
	this->_pause = pau;
}

void Engine::resolve_collision() {

	
	for (auto p1 = this->pols.begin(); p1 != pols.end(); p1++) {
		for (auto p2 = p1 + 1; p2 != pols.end(); p2++) {
			if (p1->get_immovable() && p2->get_immovable())
				continue;
			Collision_data cd = if_collide(*p1, *p2);
			collide(*p1, *p2, cd);
		}
		for (auto c1 = this->cirs.begin(); c1 != cirs.end(); c1++) {
			if (p1->get_immovable() && c1->get_immovable())
				continue;
			Collision_data cd = if_collide(*p1, *c1);
			collide(*p1, *c1, cd);
		}
	}
	for (auto c1 = this->cirs.begin(); c1 != cirs.end(); c1++) {
		for (auto c2 = c1 + 1; c2 != cirs.end(); c2++) {
			if (c1->get_immovable() && c2->get_immovable())
				continue;
			Collision_data cd = if_collide(*c1, *c2);
			collide(*c1, *c2, cd);
		}
	}
	


	/*
	auto& obja = cirs[0];
	auto& objb = pols[1];

	// mouse
	//obja.set_pos(sf::Vector2f(sf::Mouse::getPosition()));


	Collision_data cd = if_collide(obja, objb);
	collide(objb, obja, cd);

	while (!this->points.empty())
		this->points.pop_back();

	if (cd.collide) {
		Circle pt;	// illustrating contact point
		pt.set_radius(5.f);
		pt.set_pos(cd.contact_points[0]);
		pt.set_color(sf::Color::White);
		this->points.push_back(pt);
		if (cd.contact_point_count == 2) {
			pt.set_pos(cd.contact_points[1]);
			pt.set_color(sf::Color::White);
			this->points.push_back(pt);
		}
		//std::cout << "normal = (" << cd.normal[0].x << ", " << cd.normal[0].y << ")\n";
		obja.set_color(sf::Color::Red);
		objb.set_color(sf::Color::Red);

		//std::cout << "collide : " << cd.collide << "\n";
		//std::cout << "normal : (" << cd.normal.x << ", " << cd.normal.y << ")\n";
		//std::cout << "contact point : (" << cd.contact_point.x << ", " << cd.contact_point.y << ")\n";
		//std::cout << "depth : " << cd.depth << "\n";
		//system("pause");

	}
	else {
		obja.set_color(sf::Color::Green);
		objb.set_color(sf::Color::Green);
	}
	*/
}

void Engine::process_text() {
	//draw text
	std::string s;
	//s += "v = (" + std::to_string(cirs[0].get_v().x) + ", " + std::to_string(cirs[0].get_v().y) + ")\n";
	s += "E = " + std::to_string(total_E()) + "\n";

	// caculating deviation
	/*
	float theo = (float)1 / (float)2 * sfv_abs(cir->get_a()) * timer * timer;	// theoreical delta x
	float deviation = theo - cir->get_pos().x;
	s += ("\ncaculated x = " + std::to_string(theo));
	s += (", deviation = " + std::to_string(deviation) + "(" + std::to_string(deviation / theo * (float)100) + " %)\n");
	*/
	cir_pos_text.setString(s);
}

//
float Engine::total_E() {
	float E = 0.f;
	for (auto obj : cirs) {
		if (obj.get_immovable())
			continue;
		// U
		E += obj.get_mass() * g * (this->_win_height - obj.get_pos().y);
		// K
		E += 1.f / 2.f * obj.get_mass() * sfv::square(obj.get_v());	// linear
		E += 1.f / 2.f * obj.get_inertia() * sfv::square(obj.get_omega());	// rotation
	}
	for (auto obj : pols) {
		if (obj.get_immovable())
			continue;
		// U
		E += obj.get_mass() * g * (this->_win_height - obj.get_pos().y);
		// K
		E += 1.f / 2.f * obj.get_mass() * sfv::square(obj.get_v());	// linear
		E += 1.f / 2.f * obj.get_inertia() * sfv::square(obj.get_omega());	// rotation
	}
	return E;
}

// Creaters

template <class T>
bool Engine::create() {
	switch (T) {
	case Circle:
		//this->objs.push_back(Circle());
		break;
	//case Polygon:
		//this->objs.push_back(Polygon());
		//break;
	default:
		return false;
	}
	return true;
}
