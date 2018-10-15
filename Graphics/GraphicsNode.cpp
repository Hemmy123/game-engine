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

GraphicsNode::GraphicsNode(EventBus* bus, SubSystem subSystem):EventNode(bus,subSystem){
    m_renderer = new Renderer(800, 1024);
	m_perlin3D = new PerlinNoise3D(257,6);
	createDemoScene();
	
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
	
	delete m_renderer;

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
	Shader* transShader 	= new Shader(lightingVert,transFragPath);

	m_shaders.push_back(shader);
	m_shaders.push_back(transShader);
	
	m_light = new Light(Vector3(60,50,25) , Vector4(1,1,1,1), 500);

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

	m_heightMap 		= new HeightMap(rawWidth,rawHeight,heightMap_x*2,heightMap_z*2, waterHeight ,heightMap_tex_x, heightMap_tex_z,perlin);
	HeightMap* terrain 	= new HeightMap(rawWidth,rawHeight,heightMap_x,heightMap_z, 50,heightMap_tex_x, heightMap_tex_z,perlin);
	
	m_heightMap->generateRandomTerrain(Vector3(0,0,0), 3, 5, 0.5);
	terrain->generateRandomTerrain(Vector3(0,0,0), 8, 2, 0.5);

	Mesh* mesh1 = Mesh::readObjFile(MODELSDIR"Rabbit.obj");
	Mesh* mesh2 = Mesh::readObjFile(MODELSDIR"cageCube.obj");
	mesh1->loadTexture(TEXTUREDIR"Rabbit/Rabbit_D.tga");
	mesh2->loadTexture(TEXTUREDIR"nyan.jpg");
	
	terrain->loadTexture(TEXTUREDIR"Grass.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	
	m_heightMap->loadTexture(TEXTUREDIR"water.jpeg");

	m_heightMap->generateNormals();
	terrain->generateNormals();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	m_heightMap->bufferData();
	terrain->bufferData();

	
	mesh1->bufferData();
	mesh2->bufferData();

	m_meshes.push_back(mesh1);
	m_meshes.push_back(mesh2);

	// ----- Render Objects -----
	
	
	RenderObject* heightMap = new RenderObject(m_heightMap, transShader);
	//heightMap->setTransparent(true);
	RenderObject* terrainRO = new RenderObject(terrain, shader);

	RenderObject* ground = new RenderObject(mesh2, shader);

	RenderObject* ro1 = new RenderObject(mesh1, shader);
	RenderObject* ro2 = new RenderObject(mesh1, shader);
	RenderObject* ro3 = new RenderObject(mesh1, shader);
	RenderObject* ro4 = new RenderObject(mesh1, shader);

	// ----- Transformations -----
	
	Matrix4 const trans1 =  Matrix4::Translation(Vector3(0,	10,-5));
	Matrix4 const trans2 =  Matrix4::Translation(Vector3(10,10,-5));
	Matrix4 const trans3 =  Matrix4::Translation(Vector3(20,10,-5));
	Matrix4 const trans4 =  Matrix4::Translation(Vector3(30,10,-5));

	Matrix4 const cubeScale = Matrix4::Scale(Vector3(10,10,10));
	Matrix4 const cubeTrans = Matrix4::Translation(Vector3(1,-4,-5));
	
	Matrix4 const heightmapPos = Matrix4::Translation(Vector3(-20,-8,-15));
	
	Matrix4 const terrainPos = Matrix4::Translation(Vector3(-20,-5,-15));

	heightMap->setModelMatrix(heightmapPos);
	terrainRO->setModelMatrix(terrainPos);

	ground->setModelMatrix(cubeScale * cubeTrans);
	ro1->setModelMatrix(trans1 * cubeScale);
	ro2->setModelMatrix(trans2 * cubeScale);
	ro3->setModelMatrix(trans3 * cubeScale);
	ro4->setModelMatrix(trans4 * cubeScale);

	m_renderObjects.push_back(terrainRO);
	m_renderObjects.push_back(heightMap);

	
	m_renderObjects.push_back(ground);
	m_renderObjects.push_back(ro1);
	m_renderObjects.push_back(ro2);
	m_renderObjects.push_back(ro3);
	m_renderObjects.push_back(ro4);

	m_renderer->setRenderObjects(m_renderObjects);
		
	
}


void GraphicsNode::update(float msec){
	
    if (!m_renderer->checkWindow()){
		
		m_renderer->update(msec);
		counter+=(msec/40);
		
		//m_heightMap->updateTerrain(m_perlin3D,Vector3(0 ,0,counter), 4, 10, 0.5);
		m_heightMap->generateNormals();
		
		/* --- Temp lighting test --- */
		for(auto ro: m_renderer->getOpaqueObjects()){
			Shader* shader = ro->getShader();
			
			GLuint program = shader->getProgram();
			m_renderer->setShaderLight(shader, *m_light);

			glUseProgram(program);
			Vector3 cameraPos = m_renderer->getCamera()->GetPosition();
			glUniform3fv(glGetUniformLocation(program, "cameraPos"),1,(float*)&cameraPos);
		}
		
		/* ------------------------- */
		
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
		
		ObjectTag tag =obj->getTag();
		
		switch(tag){
			case T_Rabbit:{
				Mesh* rabbitMesh = Mesh::readObjFile(MODELSDIR"Rabbit.obj");
				rabbitMesh->loadTexture(TEXTUREDIR"Rabbit/Rabbit_D.tga");
				rabbitMesh->bufferData();
				m_meshes.push_back(rabbitMesh);
				RenderObject* ro1 = new RenderObject(rabbitMesh, shader);
				
				Matrix4 const pos =  Matrix4::Translation(Vector3(obj->getPosition()));
				// do scale and rotation here
				ro1->setModelMatrix(pos);
				m_renderObjects.push_back(ro1);
				m_renderer->setRenderObjects(m_renderObjects);

				break;
			}
				
			case T_Cube:{
				std::cout<< "Cube!" << std::endl;
				break;
			}
			case T_Player:{
				
			}
			case T_Wall:{
				
			}
			case T_Water:{
				
			}
				
		}
		
	
	}
	
	
}







