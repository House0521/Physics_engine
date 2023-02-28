#include "include/Engine.h"

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
