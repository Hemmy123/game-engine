#pragma once
#include "GameObject.h"
#include "GameHeightMap.h"
#include "GameLight.h"
#include "RenderObject.h"
#include "SceneManager.h"
#include "PerlinNoise2D.h"
#include "PerlinNoise3D.h"
#include "Level.h"
#include <map>
#include <unordered_map>
class LevelLoader
{
public:
	LevelLoader(SceneManager* sceneManager);
	~LevelLoader();
	Mesh* findMesh(int id);
	HeightMap* findHeightMap(int id);

	void loadLevel(Level* level);


private:

	// The scene manager to load the level to
	SceneManager*	m_sceneManager;
	PerlinNoise3D*	m_perlin3D;
	PerlinNoise2D*	m_perlin2D;
	vector<Shader*> m_shaders;
	vector<Mesh*>	m_meshes;
	vector<HeightMap*> m_heightmaps;

	// {ID : loaded}
	std::map< int, bool> m_loadedObjects;
	std::map< int, bool> m_loadedHeightmaps;

};

