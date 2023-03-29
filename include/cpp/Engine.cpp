#include <iostream>
#include <string>

#include "../Engine.h"
#include "../collision.h"
#include "../my_math.h"

//Pricate finctions
void Engine::init_vars() {
	this->_window = nullptr;

	this->_framerate = 60;
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
	// the floor
	std::vector<sf::Vector2f> floor_points;
	floor_points.push_back(sf::Vector2f(0         , _win_height - 30));
	floor_points.push_back(sf::Vector2f(_win_width, _win_height - 30));
	floor_points.push_back(sf::Vector2f(_win_width, _win_height + 50));
	floor_points.push_back(sf::Vector2f(0         , _win_height + 50));
	pols.push_back(Polygon(floor_points));
	pols[0].set_immovable(true);
	
	// circles
	
	cirs.push_back(Circle());
	cirs[0].set_radius(30.f);
	cirs[0].set_pos(sf::Vector2f(100.f, 100.f));
	

	// polygons
	std::vector<sf::Vector2f> points;
	points.push_back(sf::Vector2f(30.0, 40.0));
	points.push_back(sf::Vector2f(130.0, 140.0));
	points.push_back(sf::Vector2f(80.0, 200.0));
	pols.push_back(Polygon(points));
	
	
	std::vector<sf::Vector2f> pointss;
	pointss.push_back(sf::Vector2f(0.f, 0.f));
	pointss.push_back(sf::Vector2f(100.f, 0.f));
	pointss.push_back(sf::Vector2f(150.f, 50.f));
	pointss.push_back(sf::Vector2f(130.f, 100.f));
	pointss.push_back(sf::Vector2f(90.f, 100.f));
	pointss.push_back(sf::Vector2f(30.f, 40.f));
	pols.push_back(Polygon(pointss));
	//pols[1].set_omega(1.f);
	pols[1].set_pos(300, 300);
	
}

void Engine::init_texts() {
	if (!this->font.loadFromFile("./fonts/RobotoMono-Regular.ttf")) {
		printf("load font failed\n");
		system("pause");
	}

	this->cir_pos_text.setFont(font);
	this->cir_pos_text.setCharacterSize(20);
	cir_pos_text.setFillColor(sf::Color::White);
	cir_pos_text.setPosition(0.f, 500.f);
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

	// ----------------operation-------------------

	auto& obja = pols[1];
	auto& objb = pols[2];

	// mouse
	obja.set_pos(sf::Vector2f(sf::Mouse::getPosition()));

	
	Collision_data cd = if_collide(obja, objb);
	while(!this->points.empty())
		this->points.pop_back();

	if (cd.collide) {
		Circle pt;	// illustrating contact point
		pt.set_radius(5.f);
		pt.set_pos(cd.contact_point);
		pt.set_color(sf::Color::White);
		this->points.push_back(pt);
		//std::cout << cd.depth << "\n";

		obja.set_color(sf::Color::Red);
		objb.set_color(sf::Color::Red);
	}
	else {
		obja.set_color(sf::Color::Green);
		objb.set_color(sf::Color::Green);
	}

	// ----------------operation-------------------


	// step
	if (!this->_pause) {
		// gravity
		for (auto& cir : cirs)
			cir.set_a(sf::Vector2f(0, g) + cir.get_a());	// y positive is donwards
		for (auto& pol : pols)
			pol.set_a(sf::Vector2f(0, g) + pol.get_a());


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
	std::string s;
	//s += "v1 = (" + std::to_string(cir->get_v().x) + ", " + std::to_string(cir->get_v().y) + ")\n";
	//s += "v2 = (" + std::to_string(cir_2->get_v().x) + ", " + std::to_string(cir_2->get_v().y) + ")\n";
	//s += ("dt = " + std::to_string(dt) + ", time = " + std::to_string(timer));

	// caculating deviation
	/*
	float theo = (float)1 / (float)2 * sfv_abs(cir->get_a()) * timer * timer;	// theoreical delta x 
	float deviation = theo - cir->get_pos().x;
	s += ("\ncaculated x = " + std::to_string(theo));
	s += (", deviation = " + std::to_string(deviation) + "(" + std::to_string(deviation / theo * (float)100) + " %)\n");
	*/

	cir_pos_text.setString(s);

	this->_window->draw(cir_pos_text);

	this->_window->display();
}

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
