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

	settings.skybox			= true;
	settings.postProcessing = false;
	settings.anaglyph3D		= false;
	settings.shadows		= true;
	m_updateWater			= false;


	m_rendererController->setSetting(settings);
	//createDemoScene();
	

	m_light = new Light(Vector3(100, 500, 25), Vector4(1, 1, 1, 1), 5000);

}


GraphicsNode::~GraphicsNode(){
	
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
		updateLighting();

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
		loadLevel(m_currentLevel);
		
	}
	
}

void GraphicsNode::updateLighting()
{
	for (auto ro : m_sceneManager->getOpaque() ) {
		Shader* shader = ro->getShader();

		GLuint program = shader->getProgram();
		m_rendererController->setShaderLight(shader, m_light);

		glUseProgram(program);
		Vector3 cameraPos = m_rendererController->getCamera()->GetPosition();
		glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, (float*)&cameraPos);
	}

}

void GraphicsNode::updateWater(float msec)
{
	HeightMap* waterMesh = m_sceneManager->getWater();
	waterMesh->updateTerrain(m_perlin3D,Vector3(0 ,0,counter), 3, 10, 0.5);
	waterMesh->generateNormals();
	counter += (msec / 40);
}

void GraphicsNode::loadLevel(Level* level){
	
	std::vector<GameObject*> gameObjects = level->getGameObjects();
	
	string vertexPath 		= SHADERVERTDIR"Basic_Vert.glsl";
	string fragPath 		= SHADERFRAGDIR"Textured_Frag.glsl";
	string transFragPath 	= SHADERFRAGDIR"Trans_Frag.glsl";
	

	// Original
	//string lightingVert = SHADERVERTDIR"Lighting_Vert.glsl";
	//string lightingFrag = SHADERFRAGDIR"Lighting_Frag.glsl";
	

	// Bump testing
	//string lightingVert = SHADERVERTDIR"Bump_Vert.glsl";
	//string lightingFrag = SHADERFRAGDIR"Bump_Frag.glsl";

	// Shadow Testing 
	string lightingVert = SHADERVERTDIR"ShadowScene_Vert.glsl";
	string lightingFrag = SHADERFRAGDIR"ShadowScene_Frag.glsl";

	//Shader* shader 		= new Shader(vertexPath,fragPath);
	//Shader* transShader 	= new Shader(lightingVert,transFragPath);


	// Same value for testing
	// TODO: Change
	Shader* shader = new Shader(lightingVert, lightingFrag);
	Shader* transShader 	= new Shader(lightingVert, lightingFrag);
	
	m_shaders.push_back(shader);
	m_shaders.push_back(transShader);
	
	for(auto obj: gameObjects){
		
		ObjectTag tag = obj->getTag();
		

		switch(tag){
			case T_Rabbit:{
				Mesh* rabbitMesh = Mesh::readObjFile(MODELSDIR"Rabbit.obj");
				rabbitMesh->loadTexture(TEXTUREDIR"Rabbit/Rabbit_D.tga");
				rabbitMesh->loadBumpTexture(TEXTUREDIR"Rabbit/Rabbit_N.tga");
				rabbitMesh->generateTangents();

				rabbitMesh->bufferData();
				m_meshes.push_back(rabbitMesh);
				RenderObject* ro1 = new RenderObject(rabbitMesh, shader);

				ro1->setModelMatrix(obj->getModelMatrix());
				m_sceneManager->pushRenderObject(ro1);

				break;
			}
			case T_Terrain: {
				GameHeightMap* heightMap = static_cast<GameHeightMap*>(obj);

				HeightMap* terrain = new HeightMap(
					heightMap->getRawWidth(),
					heightMap->getRawHeight(),
					heightMap->getXMultiplier(),
					heightMap->getZMultiplier(),
					heightMap->getYMultiplier() * 4,
					heightMap->getTexCoordX(),
					heightMap->getTexCoordZ(),
					m_perlin2D );
				terrain->generateRandomTerrain(Vector3(0, 0, 0), 10, 2, 0.5);
				terrain->loadTexture(TEXTUREDIR"GrassTextures2/grass01.jpg");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				terrain->loadBumpTexture(TEXTUREDIR"GrassTextures2/grass01_n.jpg");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				terrain->generateNormals();
				terrain->generateTangents();
				terrain->bufferData();

				RenderObject* ro1 = new RenderObject(terrain, shader);
				ro1->setModelMatrix(heightMap->getModelMatrix());
				m_sceneManager->pushRenderObject(ro1);

				break;
			}
			case T_Water: {
				GameHeightMap* heightMap = static_cast<GameHeightMap*>(obj);
				heightMap->setYMulti(2);

				HeightMap* water = new HeightMap(
					heightMap->getRawWidth(),
					heightMap->getRawHeight(),
					heightMap->getXMultiplier(),
					heightMap->getZMultiplier(),
					heightMap->getYMultiplier(),
					heightMap->getTexCoordX(),
					heightMap->getTexCoordZ(),
					m_perlin2D);
				water->generateRandomTerrain(Vector3(0, 0, 0), 3, 2, 0.5);
				water->loadTexture(TEXTUREDIR"WaterTexture/water.jpg");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				water->loadBumpTexture(TEXTUREDIR"WaterTexture/waterB.jpg");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				water->generateNormals();
				water->generateTangents();
				water->bufferData();
			
				RenderObject* ro1 = new RenderObject(water, transShader);
				ro1->setModelMatrix(heightMap->getModelMatrix());
				//m_sceneManager->pushRenderObject(ro1);
				m_sceneManager->setWater(water);
				break;
			}
			case T_Cube:{
				std::cout<< "Cube!" << std::endl;
				break;
			}
			case T_Player:{
				break;
			}
			case T_Wall:{
				break;
			}	
		}
	}
}







