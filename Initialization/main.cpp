#pragma once


#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include "GraphicsNode.h"
#include "GameNode.h"
#include "EventBus.h"
#include "Timer.h"
#include "PhysicsNode.h"

int main() {
	EventBus* 			bus				= new EventBus();
	GraphicsNode* 		graphics		= new GraphicsNode(bus, Sys_Graphics);
	InterfaceHandler* 	inputHandler	= new InterfaceHandler(graphics->getWindow());
	graphics->createCamera(inputHandler);
	GameNode* 			game			= new GameNode(bus, Sys_Game, inputHandler);
	PhysicsNode*		physics			= new PhysicsNode(bus, Sys_Physics);
	Timer* 				timer			= new Timer();

	inputHandler->update();

	// 16ms
	const float MAXDT = (1.0f / 60.0f) * 1000;

	double currentTime = timer->getTime();


	while (!game->getEndGame()) {

		double newTime = timer->getTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		// std::cout << "frameTime: " << frameTime <<std::endl;

		while (frameTime > 0 && !game->getEndGame()) {
			double dt = min(frameTime, MAXDT);

			inputHandler->update();
			bus->update();

			graphics->update((float)dt);

			game->update((float)dt);


			frameTime -= dt;

		}


	}

	delete bus;
	delete game;
	delete graphics;
	delete inputHandler;
	delete timer;
	delete physics;
	return 0;
}


