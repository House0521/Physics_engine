#include "Engine.h"
#include <string>
#include <iostream>
#include "circle_collision.h"
#include "my_math.h"

//Pricate finctions
void Engine::init_vars() {
	this->window = nullptr;

	this->framerate = 1000;
	this->dt = float(1) / framerate;
	this->timer = 0;

	this->pause = false;
}

void Engine::init_window() {
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Test", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(framerate);
}

void Engine::init_objs() {
	cir = new Circle;
	cir->set_v(sf::Vector2f(50, 0));

	cir_2 = new Circle;
	cir_2->set_pos(sf::Vector2f(200, 0));
	cir_2->set_color(sf::Color::Green);

	std::vector<sf::Vector2f> points;
	points.push_back(sf::Vector2f(0.0, 0.0));
	points.push_back(sf::Vector2f(30.0, 0.0));
	points.push_back(sf::Vector2f(3.0, 30.0));
	tri = new Polygon(points);
}

void Engine::init_texts() {
	if (!this->font.loadFromFile(".\\fonts\\RobotoMono-Regular.ttf")) {
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
	delete this->window;
	delete this->cir;
	delete this->cir_2;
	delete this->tri;
}

//Accessors
const bool Engine::running() const {
	return this->window->isOpen();
}

//Funcitons
void Engine::poll_events() {
	while (this->window->pollEvent(this->ev)) {
		switch (this->ev.type) {
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Engine::update() {
	this->poll_events();

	cir->set_a(0, 0);
	cir_2->set_a(0, 0);
	if (cir_if_col(*cir, *cir_2)) {
		cir_2->set_color(sf::Color::Red);
		cir_collide(*cir, *cir_2);
	}
	else
		cir_2->set_color(sf::Color::Green);

	if (!this->pause) {
		cir->iterate(dt);
		cir_2->iterate(dt);

		timer += dt;

		//if (timer >= 5)	pause = true;
	}
	
	cir->draw();
	cir_2->draw();
	tri->draw();
}

void Engine::render() {
	this->window->clear();

	//draw object
	this->window->draw(cir->body());
	this->window->draw(cir_2->body());
	this->window->draw(tri->body());

	//draw text
	std::string s;
	s += "v1 = (" + std::to_string(cir->get_v().x) + ", " + std::to_string(cir->get_v().y) + ")\n";
	s += "v2 = (" + std::to_string(cir_2->get_v().x) + ", " + std::to_string(cir_2->get_v().y) + ")\n";
	s += ("dt = " + std::to_string(dt) + ", time = " + std::to_string(timer));

	// caculating deviation
	/*
	float theo = (float)1 / (float)2 * sfv_abs(cir->get_a()) * timer * timer;	// theoreical delta x 
	float deviation = theo - cir->get_pos().x;
	s += ("\ncaculated x = " + std::to_string(theo));
	s += (", deviation = " + std::to_string(deviation) + "(" + std::to_string(deviation / theo * (float)100) + " %)\n");
	*/

	cir_pos_text.setString(s);

	this->window->draw(cir_pos_text);

	this->window->display();
}
