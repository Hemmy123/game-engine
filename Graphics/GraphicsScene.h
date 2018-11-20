#pragma once

#include "RenderObject.h"

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

	void loadScene1();
	void loadScene2();
	void loadScene3();

	RenderObject* getScene(int i);

private:
	RenderObject* m_parent;
	RenderObject* m_scene1;
	RenderObject* m_scene2;
	RenderObject* m_scene3;
	
};

