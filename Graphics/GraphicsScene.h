#pragma once

/*
Scene that holds a bunch of objects and is meant to be displayed 
using the renderer. 

Note: This is mainly for the graphics to quickly switch between 
scene to show off the graphics engine. In a real setting, scenes
(or levels) would be loaded from the game subsystem
*/
class GraphicsScene
{
public:
	GraphicsScene();
	~GraphicsScene();
};

