#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "Shape.h"
#include "Circle.h"
#include "Polygon.h"


/*
	Dominate Class
*/



class Engine {
private:
	//Vars
	int framerate;
	float dt, timer;
	bool pause;

	//Window
	sf::RenderWindow* window;
	sf::Event ev;
	sf::VideoMode videoMode;

	//Private finctions
	void init_vars();
	void init_window();
	void init_objs();
	void init_texts();

	//Objects
	Circle* cir, *cir_2;
	Polygon* tri;

	//Text
	sf::Text cir_pos_text;
	sf::Font font;

public:
	//Constructors / Destructors
	Engine();
	~Engine();

	//Accessors
	const bool running() const;

	//Functions
	void poll_events();
	void update();
	void render();
};


#endif // ENGINE_H