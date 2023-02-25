#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>


#include "Engine.h"

int main()
{
	//Init engine
	Engine engine;

	//Loop
	while(engine.running()){
		engine.update();
		engine.render();
	}
	

	//End
    return 0;
}
