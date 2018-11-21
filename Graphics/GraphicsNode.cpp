//
//  GraphicsNode.cpp
//  Graphics
//
//  Created by Hemmy on 30/05/2018.
//  Copyright © 2018 Hemmy. All rights reserved.
//

#include "GraphicsNode.h"

#include "SOIL2.h"
#include "FilePaths.h"
#include "PerlinNoise2D.h"
#include "GameHeightMap.h"

GraphicsNode::GraphicsNode(EventBus* bus, SubSystem subSystem):
	EventNode(bus,subSystem){
	m_sceneManager = new SceneManager();
	m_rendererController = new RendererController(1000, 1600, m_sceneManager);
	m_perlin3D = new PerlinNoise3D(257,6);
	m_perlin2D = new PerlinNoise2D(257,6);
	RendererSettings settings;

	m_updateWater = false;
}


GraphicsNode::~GraphicsNode(){
	
	delete m_levelLoader;

	for(auto shader: m_shaders){
		delete shader;
	}
	
	for(auto mesh: m_meshes){
		delete mesh;
	}
	
	for(auto renderObject: m_renderObjects){
		delete renderObject;
	}

	for (auto light : m_lights) {
		delete light;
	}
	
	delete m_currentLevel;
	delete m_sceneManager;
	delete m_perlin3D;
	delete m_rendererController;

}


void GraphicsNode::update(float msec){
	
    if (!m_rendererController->checkWindow()){
		m_cameraController->update(msec);
		m_rendererController->update(msec);
		if (m_sceneManager->getWater() != nullptr && m_updateWater) {
			updateWater(msec);
		}	
    }
}


void GraphicsNode::handleEvent(Event event){
	SubSystem sender = event.getSender();
	SubSystem receiver = event.getReceiver();
	std::string type = event.getType();
	
	if(sender ==  Sys_Game && receiver == Sys_Graphics && type == "Load_Level"){
		m_currentLevel = static_cast<Level*>(event.getData());
		//loadLevel(m_currentLevel);
		m_levelLoader->loadLevel(m_currentLevel);
		m_rendererController->setSetting(m_sceneManager->getSettings());

	}
	
}



void GraphicsNode::updateWater(float msec)
{
	RenderObject* waterObj = m_sceneManager->getWater();
	HeightMap*	waterMesh = static_cast<HeightMap*>(waterObj->getMesh());

	waterMesh->updateTerrain(m_perlin3D,Vector3(0 ,0,counter), 3, 10, 0.5);
	waterMesh->generateNormals();
	counter += (msec / 40);
}






