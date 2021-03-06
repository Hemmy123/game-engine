//
//  GraphicsNode.h
//  Graphics
//
//  Created by Hemmy on 30/05/2018.
//  Copyright © 2018 Hemmy. All rights reserved.
//
#pragma once


#include <stdio.h>
#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "RenderObject.h"
#include "GameObject.h"

#include "EventNode.h"
#include "HeightMap.h"
#include "Level.h"
#include "RendererController.h"
#include "SceneManager.h"
#include "LevelLoader.h"
#include "CameraController.h"
#include "Timer.h"

using std::vector;

class GraphicsNode:public EventNode {
public:
    GraphicsNode(EventBus* eventBus, SubSystem subSystem);
    ~GraphicsNode();
	

	void handleEvent(Event event);
    void update(float msec);

	void createCamera(InterfaceHandler* ih) {
		m_rendererController->createCamera(ih);
		m_cameraController = new CameraController(m_rendererController->getCamera());
		m_levelLoader = new LevelLoader(m_sceneManager, m_cameraController);

	
	}
	GLFWwindow* getWindow() const { return m_rendererController->getWindow(); }

	void updateWater(float msec);
	
private:


	double	m_startFrameTime;
	double	m_endFrameTime;
	double	m_lastFrameTime;

	float	m_fps;
	Timer	m_timer;

	SceneManager*		m_sceneManager;
	RendererController* m_rendererController;
	LevelLoader*		m_levelLoader;
	CameraController*	m_cameraController;

	bool m_updateWater;
	HeightMap* m_water;
	float counter = 0;

	PerlinNoise3D* m_perlin3D;
	PerlinNoise2D* m_perlin2D;

	vector<Shader*> 		m_shaders;
	vector<Mesh*>			m_meshes;
	vector<RenderObject*> 	m_renderObjects;
	vector<Light*>			m_lights;
	
	Level*		m_currentLevel;
	Light* 		m_light;

    
};




 /* GraphicsNode_hpp */
