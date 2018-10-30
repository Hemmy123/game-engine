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


using std::vector;

class GraphicsNode:public EventNode {
public:
    GraphicsNode(EventBus* eventBus, SubSystem subSystem);
    ~GraphicsNode();
	
	void initOGL();
	void createDemoScene();
	void handleEvent(Event event);
    void update(float msec);
	
	//void createCamera(InterfaceHandler* ih) { m_renderer->createCamera(ih);}
	//GLFWwindow* getWindow() const {return m_renderer->getWindow();}

	void createCamera(InterfaceHandler* ih) { m_rendererController->createCamera(ih);}
	GLFWwindow* getWindow() const { return m_rendererController->getWindow(); }

	
	void updateLighting();
	void loadLevel(Level* level);
	
private:
	
	RendererController* m_rendererController;
    //Renderer *m_renderer;
	
	/*----- Testing ------------*/
	
	HeightMap* m_heightMap;
	float counter = 0;
	PerlinNoise3D* m_perlin3D;
	/*--------------------------*/

	
	
	vector<Shader*> 		m_shaders;
	vector<Mesh*>			m_meshes;
	vector<RenderObject*> 	m_renderObjects;
	vector<Light*>			m_lights;
	
	Level*		m_currentLevel;
	Light* 		m_light;

    
};




 /* GraphicsNode_hpp */
