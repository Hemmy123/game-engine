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
	bool skybox;
	bool postProcessing;
	bool anaglyph3D;
	bool shadows;
	bool basicLighting;	// true for basic
	bool differedRendering;
};


class Level {
public:

	Level();
	~Level();

	std::vector<GameObject*> getGameObjects() { return m_gameObjects; }


	void createDeferredLevelDemo();
	void createDemoLevel();
	RendererSettings getSettings() const { return m_rendererSettings; };

private:
	// Shouldn't really combine graphics logic with 
	// level logic, but it will have to do for the
	// graphics coursework
	RendererSettings m_rendererSettings;
	std::vector<GameObject*> m_gameObjects;
};

