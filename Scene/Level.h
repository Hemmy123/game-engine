////========================================
// Class: Level
// Author: Hemmy
// Date: 11/07/2018
// Description:
// A class that holds a bunch of gameobjects
// that are to be loaded in my the graphics
// and physics subsystems
// ========================================

#pragma once

#include <stdio.h>
#include <vector>
#include "GameObject.h"


// A simple struct to toggle features on and off
struct RendererSettings {
	RendererSettings() {
		init();
	};

	bool skybox;
	bool postProcessing;
	bool anaglyph3D;
	bool shadows;
	bool basicLighting;	// true for basic
	bool deferredRendering;

	void init() {
		skybox				= false;
		postProcessing		= false;
		anaglyph3D			= false;
		shadows				= false;
		basicLighting		= false;
		deferredRendering	= false;
	}

};


class Level {
public:

	Level();
	~Level();

	std::vector<GameObject*> getGameObjects() { return m_gameObjects; }

	void createScene3();
	void createScene2();
	void createScene1();

	RendererSettings getSettings() const { return m_rendererSettings; };
	int getID() const { return id; }
private:
	// Shouldn't really combine graphics logic with 
	// level logic, but it will have to do for the
	// graphics coursework
	RendererSettings m_rendererSettings;
	int id;
	std::vector<GameObject*> m_gameObjects;
};

