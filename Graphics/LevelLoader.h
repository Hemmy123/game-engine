#pragma once
#include "GameObject.h"
#include "GameHeightMap.h"
#include "RenderObject.h"
#include "SceneManager.h"
#include "PerlinNoise2D.h"
#include "PerlinNoise3D.h"
#include "Level.h"

class LevelLoader
{
public:
	LevelLoader(SceneManager* sceneManager);
	~LevelLoader();

	void loadLevel(Level* level);


private:

	// The scene manager to load the level to
	SceneManager*	m_sceneManager;
	PerlinNoise3D*	m_perlin3D;
	PerlinNoise2D*	m_perlin2D;
	vector<Shader*> m_shaders;
	vector<Mesh*>	m_meshes;
};

