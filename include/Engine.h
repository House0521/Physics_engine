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
	int _framerate;	// update frequency
	float _dt, _timer, _pause_time;
	bool _pause, _gravity;

	//constants
	float g = 500;

	//Window
	float _win_height = 600, _win_width = 800;
	sf::RenderWindow* _window;
	sf::Event _ev;
	sf::VideoMode _videoMode;

	//Private finctions
	void init_vars();
	void init_window();
	void init_objs();
	void init_texts();

	//Objects
	std::vector<Circle> cirs;
	std::vector<Polygon> pols;
	std::vector<Circle> points;

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
	void pause(bool pau);
	void resolve_collision();
	void process_text();

	//
	float total_E();

	//Creaters
	template <class T>
	bool create();
};


#endif // ENGINE_H