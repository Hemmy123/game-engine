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


void GraphicsNode::createDemoScene(){

	
	// ----- Create Shaders -----
	string vertexPath 		= SHADERVERTDIR"Basic_Vert.glsl";
	string fragPath 		= SHADERFRAGDIR"Textured_Frag.glsl";
	string transFragPath 	= SHADERFRAGDIR"Trans_Frag.glsl";
	
	string lightingVert = SHADERVERTDIR"Lighting_Vert.glsl";
	string lightingFrag = SHADERFRAGDIR"Lighting_Frag.glsl";
	
	//Shader* shader 		= new Shader(vertexPath,fragPath);
	Shader* shader 			= new Shader(lightingVert,lightingFrag);
	Shader* transparentShader 	= new Shader(lightingVert,transFragPath);

	m_shaders.push_back(shader);
	m_shaders.push_back(transparentShader);
	

	// ----- Create Meshes -----
	int rawWidth = 257;
	int rawHeight = 257;
	float heightMap_x = 1;
	float heightMap_z = 1;
	float heightMap_y = 10;
	float heightMap_tex_x = 1/heightMap_x;
	float heightMap_tex_z = 1/heightMap_z;
	
	PerlinNoise2D* perlin = new PerlinNoise2D(rawWidth,6);

	float waterHeight = 5.0f;

	m_water 		= new HeightMap(rawWidth,rawHeight,heightMap_x*2,heightMap_z*2, waterHeight ,heightMap_tex_x, heightMap_tex_z,perlin);
	HeightMap* terrain 	= new HeightMap(rawWidth,rawHeight,heightMap_x,heightMap_z, 50,heightMap_tex_x, heightMap_tex_z,perlin);
	
	m_water->generateRandomTerrain(Vector3(0,0,0), 3, 5, 0.5);
	terrain->generateRandomTerrain(Vector3(0,0,0), 8, 2, 0.5);

	Mesh* rabbitMesh = Mesh::readObjFile(MODELSDIR"Rabbit.obj");
	Mesh* cubeMesh = Mesh::readObjFile(MODELSDIR"cageCube.obj");
	Mesh* carMesh = Mesh::readObjFile(MODELSDIR"Lamborghini_Aventador.obj");



	rabbitMesh->loadTexture(TEXTUREDIR"Rabbit/Rabbit_D.tga");
	cubeMesh->loadTexture(TEXTUREDIR"nyan.jpg");
	carMesh->loadTexture(TEXTUREDIR"car/Lamborginhi Aventador_diffuse.jpeg");
	
	terrain->loadTexture(TEXTUREDIR"Grass.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	
	m_water->loadTexture(TEXTUREDIR"water.jpeg");

	m_water->generateNormals();
	terrain->generateNormals();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	m_water->bufferData();
	terrain->bufferData();

	
	rabbitMesh->bufferData();
	cubeMesh->bufferData();
	carMesh->bufferData();

	m_meshes.push_back(rabbitMesh);
	m_meshes.push_back(cubeMesh);
	m_meshes.push_back(carMesh);

	// ----- Render Objects -----
	
	
	RenderObject* heightMap = new RenderObject(m_water, transparentShader);
	//heightMap->setTransparent(true);
	RenderObject* terrainRO = new RenderObject(terrain, shader);

	RenderObject* ground = new RenderObject(cubeMesh, shader);

	RenderObject* rabbit1 = new RenderObject(rabbitMesh, shader);
	RenderObject* rabbit2 = new RenderObject(rabbitMesh, shader);
	RenderObject* rabbit3 = new RenderObject(rabbitMesh, shader);
	RenderObject* rabbit4 = new RenderObject(rabbitMesh, shader);


	

	// ----- Transformations -----
	
	Matrix4 const trans1 =  Matrix4::Translation(Vector3(0,	10,-5));
	Matrix4 const trans2 =  Matrix4::Translation(Vector3(10,10,-5));
	Matrix4 const trans3 =  Matrix4::Translation(Vector3(20,10,-5));
	Matrix4 const trans4 =  Matrix4::Translation(Vector3(30,10,-5));

	Matrix4 const cubeScale = Matrix4::Scale(Vector3(10,10,10));
	Matrix4 const cubeTrans = Matrix4::Translation(Vector3(1,-4,-5));
	
	Matrix4 const heightmapPos = Matrix4::Translation(Vector3(-20,-8,-15));
	Matrix4 const heightmapScale = Matrix4::Scale(Vector3(10,10,10));
	Matrix4 const terrainPos = Matrix4::Translation(Vector3(-20,-5,-15));

	heightMap->setModelMatrix(heightmapPos *heightmapScale);
	terrainRO->setModelMatrix(terrainPos *heightmapScale);

	ground->setModelMatrix(cubeScale * cubeTrans);
	rabbit1->setModelMatrix(trans1 * cubeScale);
	rabbit2->setModelMatrix(trans2 * cubeScale);
	rabbit3->setModelMatrix(trans3 * cubeScale);
	rabbit4->setModelMatrix(trans4 * cubeScale);

	m_sceneManager->pushRenderObject(terrainRO);
	m_sceneManager->pushRenderObject(heightMap);
	m_sceneManager->pushRenderObject(ground);
	m_sceneManager->pushRenderObject(rabbit1);
	m_sceneManager->pushRenderObject(rabbit2);
	m_sceneManager->pushRenderObject(rabbit3);
	m_sceneManager->pushRenderObject(rabbit4);

		
	
}


void GraphicsNode::update(float msec){
	
    if (!m_rendererController->checkWindow()){
		updateLighting();

		m_rendererController->update(msec);
		counter+=(msec/40);
		if (m_sceneManager->getWater() != nullptr) {
			//updateWater();
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

void GraphicsNode::updateWater()
{
	HeightMap* waterMesh = m_sceneManager->getWater();
	waterMesh->updateTerrain(m_perlin3D,Vector3(0 ,0,counter), 3, 10, 0.5);
	waterMesh->generateNormals();
}

void GraphicsNode::loadLevel(Level* level){
	
	std::vector<GameObject*> gameObjects = level->getGameObjects();
	
	string vertexPath 		= SHADERVERTDIR"Basic_Vert.glsl";
	string fragPath 		= SHADERFRAGDIR"Textured_Frag.glsl";
	string transFragPath 	= SHADERFRAGDIR"Trans_Frag.glsl";
	
	string lightingVert = SHADERVERTDIR"Lighting_Vert.glsl";
	string lightingFrag = SHADERFRAGDIR"Lighting_Frag.glsl";
	
	//Shader* shader 		= new Shader(vertexPath,fragPath);
	Shader* shader 			= new Shader(lightingVert,lightingFrag);
	Shader* transShader 	= new Shader(lightingVert,transFragPath);
	
	m_shaders.push_back(shader);
	m_shaders.push_back(transShader);
	
	for(auto obj: gameObjects){
		
		ObjectTag tag = obj->getTag();
		

		switch(tag){
			case T_Rabbit:{
				Mesh* rabbitMesh = Mesh::readObjFile(MODELSDIR"Rabbit.obj");
				rabbitMesh->loadTexture(TEXTUREDIR"Rabbit/Rabbit_D.tga");
				rabbitMesh->bufferData();
				m_meshes.push_back(rabbitMesh);
				RenderObject* ro1 = new RenderObject(rabbitMesh, shader);

				ro1->setModelMatrix(obj->getModelMatrix());
				//m_renderObjects.push_back(rabbit1);
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
				terrain->generateRandomTerrain(Vector3(0, 0, 0), 8, 2, 0.5);
				terrain->loadTexture(TEXTUREDIR"Grass.jpg");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				terrain->generateNormals();
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
				water->loadTexture(TEXTUREDIR"water.jpeg");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				water->generateNormals();
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







