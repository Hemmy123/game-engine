#include "LevelLoader.h"
#include "FilePaths.h"

LevelLoader::LevelLoader(SceneManager * sceneManager, CameraController* controller):
	m_sceneManager(sceneManager),
	m_cameraController(controller)
{
	m_perlin3D = new PerlinNoise3D(257, 6);
	m_perlin2D = new PerlinNoise2D(257, 6);

	m_perlinNoiseShader = new Shader(SHADERVERTDIR"PerlinNoise3D_Vert.glsl", SHADERFRAGDIR"Reflect_Frag.glsl");


	for (int i = T_StartTag; i < T_EndTag; i++) {
		m_loadedObjects.insert({ i, false });
	}for (int i = T_StartTag; i <= T_Water; i++) {
		m_loadedHeightmaps.insert({ i, false });
	}



}

LevelLoader::~LevelLoader()
{
	for (auto shader : m_shaders) {
		delete shader;
	}
	m_shaders.clear();

	for (auto mesh : m_meshes) {
		delete mesh;
	}
	m_meshes.clear();

	for (auto hm : m_heightmaps) {
		delete hm;
	}
	m_heightmaps.clear();

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

HeightMap * LevelLoader::findHeightMap(int id)
{
	for (auto hm : m_heightmaps) {
		if (hm->getId() == id) {
			return hm;
		}
	}
}

void LevelLoader::loadLevel(Level * level)
{
	m_sceneManager->clearAllObjects();
	

	std::vector<GameObject*> gameObjects = level->getGameObjects();
	m_sceneManager->setSettings(level->getSettings());
	m_sceneManager->setSceneID(level->getID());
	m_cameraController->loadScenePositions(level->getID());
	string vertexPath	= SHADERVERTDIR"Basic_Vert.glsl";
	string fragPath		= SHADERFRAGDIR"Textured_Frag.glsl";
	string transFragPath = SHADERFRAGDIR"Trans_Frag.glsl";


	// Original
	//string lightingVert = SHADERVERTDIR"Lighting_Vert.glsl";
	//string lightingFrag = SHADERFRAGDIR"Lighting_Frag.glsl";

	// Shadow Testing 
	string lightingVert = SHADERVERTDIR"ShadowScene_Vert.glsl";
	string lightingFrag = SHADERFRAGDIR"ShadowScene_Frag.glsl";


	Shader* shader = new Shader(lightingVert, lightingFrag);
	Shader* transShader = new Shader(lightingVert, lightingFrag);

	m_shaders.push_back(shader);
	m_shaders.push_back(transShader);

	for (auto obj : gameObjects) {

		ObjectTag tag = obj->getTag();
		float scaleAverage =
			(obj->getScale().x +
				obj->getScale().y +
				obj->getScale().z) / 3;

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
			

			ro1->setBoundingRadius(scaleAverage);
			m_sceneManager->pushRenderObject(ro1);

			break;
		}
		case T_Terrain: {
			GameHeightMap* heightMap = static_cast<GameHeightMap*>(obj);
						
			HeightMap* terrain;
			Vector3 position = heightMap->getPosition();

			// Need to find a better way of loading Terrains...
			if (m_loadedHeightmaps.at(T_Terrain) != true) {

				terrain = new HeightMap(
					heightMap->getRawWidth(),
					heightMap->getRawHeight(),
					heightMap->getXMultiplier(),
					heightMap->getZMultiplier(),
					heightMap->getYMultiplier() * 4,
					heightMap->getTexCoordX(),
					heightMap->getTexCoordZ(),
					m_perlin2D);
				terrain->setId(T_Terrain);
				m_heightmaps.push_back(terrain);
				float minHeight = -0.3;
				float maxHeight = 0.5;
				terrain->generateRandomTerrain(Vector3(0, 0, 0), 6, 2, 0.5, minHeight, maxHeight);

				terrain->loadTexture(TEXTUREDIR"GrassTextures2/grass01.jpg");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				terrain->loadBumpTexture(TEXTUREDIR"GrassTextures2/grass01_n.jpg");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				terrain->generateNormals();
				terrain->generateTangents();
				terrain->bufferData();
				
				m_loadedHeightmaps[T_Terrain] = true;
			}
			else {
				terrain = findHeightMap(T_Terrain);
			}
			

			RenderObject* ro1 = new RenderObject(terrain, shader);
			ro1->setModelMatrix(heightMap->getModelMatrix());


			ro1->setBoundingRadius(heightMap->getRawWidth() *heightMap->getXMultiplier());
			m_sceneManager->pushRenderObject(ro1);

			break;
		}
		case T_Water: {
			GameHeightMap* heightMap = static_cast<GameHeightMap*>(obj);
			heightMap->setYMulti(2);

			HeightMap* water;
			// Need to find a better way of loading Terrains...
			if (m_loadedHeightmaps.at(T_Water) != true) {

				water = new HeightMap(
					heightMap->getRawWidth(),
					heightMap->getRawHeight(),
					heightMap->getXMultiplier(),
					heightMap->getZMultiplier(),
					heightMap->getYMultiplier(),
					heightMap->getTexCoordX(),
					heightMap->getTexCoordZ(),
					m_perlin2D);
				water->setId(T_Water);
				m_heightmaps.push_back(water);
				float minHeight = -0.3;
				float maxHeight = 0.5;
				//water->generateRandomTerrain(Vector3(0, 0, 0), 3, 2, 0.5, 0, 1);
				water->generateFlatTerrain();

				water->loadTexture(TEXTUREDIR"WaterTexture/water.jpg");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				water->loadBumpTexture(TEXTUREDIR"WaterTexture/waterB.jpg");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				water->generateNormals();
				water->generateTangents();
				water->bufferData();

				m_loadedHeightmaps[T_Water] = true;
			}
			else {
				water = findHeightMap(T_Water);
			}

			RenderObject* ro1 = new RenderObject(water, m_perlinNoiseShader);
			ro1->setBoundingRadius(heightMap->getRawWidth() *heightMap->getXMultiplier());

			ro1->setModelMatrix(heightMap->getModelMatrix());
			ro1->setTransparent(true);
			m_sceneManager->setWater(ro1);
			break;
		}
		case T_SpotLight: {
			GameLight* gameLight = static_cast<GameLight*>(obj);

			Mesh* lightMesh	;
			if (m_loadedObjects.at(T_SpotLight) != true) {
				lightMesh = Mesh::readObjFile(MODELSDIR"cone.obj");
				lightMesh->setId(T_SpotLight);
				lightMesh->bufferData();
				m_loadedObjects.at(T_SpotLight) = true;
				m_meshes.push_back(lightMesh);

			}
			else {
				lightMesh = findMesh(T_SpotLight);
			}
			Light* light = new Light(gameLight->getPosition(), gameLight->getColour(), gameLight->getScale());

			light->setMesh(lightMesh);
			m_sceneManager->pushLight(light);

			break;
		}
		case T_PointLight: {

			GameLight* gameLight = static_cast<GameLight*>(obj);

			Mesh* lightMesh;
			if (m_loadedObjects.at(T_PointLight) != true) {
				lightMesh = Mesh::readObjFile(MODELSDIR"ico.obj");
				lightMesh->setId(T_PointLight);
				lightMesh->bufferData();
				m_loadedObjects.at(T_PointLight) = true;
				m_meshes.push_back(lightMesh);

			}
			else {
				lightMesh = findMesh(T_PointLight);
			}
			Light* light = new Light(gameLight->getPosition(), gameLight->getColour(), gameLight->getRadius());

			light->setMesh(lightMesh);
			m_sceneManager->pushLight(light);
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
