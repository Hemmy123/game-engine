#include "LevelLoader.h"
#include "FilePaths.h"

LevelLoader::LevelLoader(SceneManager * sceneManager):
	m_sceneManager(sceneManager)
{
	m_perlin3D = new PerlinNoise3D(257, 6);
	m_perlin2D = new PerlinNoise2D(257, 6);


	for (int i = T_StartTag; i < T_EndTag; i++) {
		m_loadedObjects.insert({ i, false });
	}

}

LevelLoader::~LevelLoader()
{
	for (auto shader : m_shaders) {
		delete shader;
	}

	for (auto mesh : m_meshes) {
		delete mesh;
	}

	delete m_perlin2D;
	delete m_perlin3D;

}

Mesh * LevelLoader::findMesh(int id)
{
	for (auto mesh : m_meshes) {
		if (mesh->getId() == id) {
			return mesh;
		}
	}
}

void LevelLoader::loadLevel(Level * level)
{
	m_sceneManager->clearAllObjects();

	std::vector<GameObject*> gameObjects = level->getGameObjects();
	m_sceneManager->setSettings(level->getSettings());

	string vertexPath	= SHADERVERTDIR"Basic_Vert.glsl";
	string fragPath		= SHADERFRAGDIR"Textured_Frag.glsl";
	string transFragPath = SHADERFRAGDIR"Trans_Frag.glsl";


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
	Shader* transShader = new Shader(lightingVert, lightingFrag);

	m_shaders.push_back(shader);
	m_shaders.push_back(transShader);

	for (auto obj : gameObjects) {

		ObjectTag tag = obj->getTag();


		switch (tag) {
		case T_Rabbit: {

			Mesh* rabbitMesh;
			if (m_loadedObjects.at(T_Rabbit) != true) {
				rabbitMesh = Mesh::readObjFile(MODELSDIR"Rabbit.obj");
				rabbitMesh->setId(T_Rabbit);
				rabbitMesh->loadTexture(TEXTUREDIR"Rabbit/Rabbit_D.tga");
				rabbitMesh->loadBumpTexture(TEXTUREDIR"Rabbit/Rabbit_N.tga");
				rabbitMesh->generateTangents();
				rabbitMesh->bufferData();

				m_loadedObjects.at(T_Rabbit) = true;
				m_meshes.push_back(rabbitMesh);

			}
			else {
				rabbitMesh = findMesh(T_Rabbit);
			}
		
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
				m_perlin2D);

			float minHeight = -0.3;
			float maxHeight= 0.5;
			terrain->generateRandomTerrain(Vector3(0, 0, 0), 10, 2, 0.5, minHeight, maxHeight);
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
			water->generateRandomTerrain(Vector3(0, 0, 0), 3, 2, 0.5, 0, 1);
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
			ro1->setTransparent(true);
			m_sceneManager->setWater(ro1);
			break;
		}
		case T_Light: {
			GameLight* gameLight = static_cast<GameLight*>(obj);

			Mesh* lightMesh	;
			if (m_loadedObjects.at(T_Light) != true) {
				lightMesh = Mesh::readObjFile(MODELSDIR"ico.obj");
				lightMesh->setId(T_Light);
				lightMesh->bufferData();
				m_loadedObjects.at(T_Light) = true;
				m_meshes.push_back(lightMesh);

			}
			else {
				lightMesh = findMesh(T_Light);
			}

			Light* light = new Light(gameLight->getPosition(), gameLight->getColour(), gameLight->getRadius());
			light->setMesh(lightMesh);
			m_sceneManager->pushLight(light);

			break;
		}
		case T_Cube: {
			break;
		}
		case T_Player: {
			break;
		}
		case T_Street: {

			Mesh* streetMesh;
			if (m_loadedObjects.at(T_Street) != true) {
				streetMesh = Mesh::readObjFile(MODELSDIR"Street/StreetExport1.obj");
				streetMesh->setId(T_Street);
				//streetMesh->loadTexture(TEXTUREDIR"Street/StreetExportobj.png");
				//streetMesh->loadBumpTexture(TEXTUREDIR"Street/Rabbit_N.tga");
				streetMesh->generateTangents();
				streetMesh->bufferData();

				m_loadedObjects.at(T_Street) = true;
				m_meshes.push_back(streetMesh);

			}
			else {
				streetMesh = findMesh(T_Street);
			}

			RenderObject* ro1 = new RenderObject(streetMesh, shader);

			ro1->setModelMatrix(obj->getModelMatrix());
			m_sceneManager->pushRenderObject(ro1);

			break;

		}
		}
	}

}
