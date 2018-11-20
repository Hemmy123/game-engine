////========================================
// Class: Level
// Author: Hemmy
// Date: 11/07/2018
// Description:
//
// ========================================

#include "Level.h"
#include "GameHeightMap.h"
#include "GameLight.h"

Level::Level() {

}

Level::~Level() {
	for (auto gameObj : m_gameObjects) {
		delete gameObj;
	}
	m_gameObjects.clear();
}

void Level::createScene2()
{

	m_rendererSettings.skybox				= true;
	m_rendererSettings.postProcessing		= false;
	m_rendererSettings.anaglyph3D			= false;
	m_rendererSettings.shadows				= false;
	m_rendererSettings.basicLighting		= false;
	m_rendererSettings.deferredRendering	= true;

	// ===== Lights ===== //

	const int rows			= 10;
	const int columns		= 5;
	const int totalLights	= rows * columns;
	
	
	float lightRadius	= 150;
	Vector3 scale(100, 500, 100);
	

	float lightOffset	= 500;

	Vector3 origin(400, 230, 100);

	Vector3 lightPos[totalLights];


	// Create positions
	for (int x = 0; x < columns; x++) {
		for (int z = 0; z < rows; z++) {

			int offset = (x * rows) + z;
			Vector3 pos(x * lightOffset, 0, z * lightOffset);
			pos = pos + origin;
			lightPos[offset] = pos;
		}
	}



	// create light with positions
	for (int i = 0; i < totalLights; i++) {
		 Vector3 light_pos = lightPos[i];

		float r = 0.5f + (float)(rand() % 129) / 128.0f;
		float g = 0.5f + (float)(rand() % 129) / 128.0f;
		float b = 0.5f + (float)(rand() % 129) / 128.0f;
		Vector4 lightColour(r, g, b, 1);

		//Vector3 light_pos(i * lightOffset, height, i * lightOffset );

		GameLight* light = new GameLight(light_pos, lightColour, scale);
		light->setTag(T_SpotLight);
		m_gameObjects.push_back(light);
	}


	GameHeightMap* terrain = new GameHeightMap(Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(10, 10, 10));
	terrain->setTag(T_Terrain);
	m_gameObjects.push_back(terrain);

	




}

void Level::createScene1() {

	// Hard coding a level for coursework. Should really be data driven later

	m_rendererSettings.skybox				= true;
	m_rendererSettings.postProcessing		= false;
	m_rendererSettings.anaglyph3D			= false;
	m_rendererSettings.shadows				= true;
	m_rendererSettings.basicLighting		= true;
	m_rendererSettings.deferredRendering	= false;

	 // ===== Rabbits ===== //
	Vector3 obj1_scale(30, 30, 30);

	int obj1_offSet = 35;
	int obj1_y = 195;
	int obj1_x = 200;
	int obj1_z = 300; 

	GameObject* rabbit1 = new GameObject(Vector3(obj1_x + -obj1_offSet * 2, obj1_y, obj1_z), Vector3(1, 1, 1), obj1_scale);
	GameObject* rabbit2 = new GameObject(Vector3(obj1_x + -obj1_offSet, obj1_y, obj1_z), Vector3(1, 1, 1), obj1_scale);
	GameObject* rabbit3 = new GameObject(Vector3(obj1_x + 0, obj1_y, obj1_z), Vector3(1, 1, 1), obj1_scale);

	GameObject* rabbit4 = new GameObject(Vector3(obj1_x + -obj1_offSet * 2, obj1_y, obj1_z + obj1_offSet), Vector3(1, 1, 1), obj1_scale);
	GameObject* rabbit5 = new GameObject(Vector3(obj1_x + -obj1_offSet, obj1_y, obj1_z + obj1_offSet), Vector3(1, 1, 1), obj1_scale);
	GameObject* rabbit6 = new GameObject(Vector3(obj1_x + 0, obj1_y, obj1_z + obj1_offSet), Vector3(1, 1, 1), obj1_scale);

	GameObject* rabbit7 = new GameObject(Vector3(obj1_x + -obj1_offSet * 2, obj1_y, obj1_z + (2 * obj1_offSet)), Vector3(1, 1, 1), obj1_scale);
	GameObject* rabbit8 = new GameObject(Vector3(obj1_x + -obj1_offSet, obj1_y, obj1_z + (2 * obj1_offSet)), Vector3(1, 1, 1), obj1_scale);
	GameObject* rabbit9 = new GameObject(Vector3(obj1_x + 0, obj1_y, obj1_z + (2 * obj1_offSet)), Vector3(1, 1, 1), obj1_scale);
	
	GameObject* bin = new GameObject(Vector3(obj1_x + 0, obj1_y + 10, obj1_z + (2 * obj1_offSet + 40)), Vector3(1, 1, 1), Vector3(2,2,2));

	rabbit1->setTag(T_Rabbit);
	rabbit2->setTag(T_Rabbit);
	rabbit3->setTag(T_Rabbit);
	rabbit4->setTag(T_Rabbit);
	rabbit5->setTag(T_Rabbit);
	rabbit6->setTag(T_Rabbit);
	rabbit7->setTag(T_Rabbit);
	rabbit8->setTag(T_Rabbit);
	rabbit9->setTag(T_Rabbit);
	bin->setTag(T_Street);

	m_gameObjects.push_back(rabbit1);
	m_gameObjects.push_back(rabbit2);
	m_gameObjects.push_back(rabbit3);
	m_gameObjects.push_back(rabbit4);
	m_gameObjects.push_back(rabbit5);
	m_gameObjects.push_back(rabbit6);
	m_gameObjects.push_back(rabbit7);
	m_gameObjects.push_back(rabbit8);
	m_gameObjects.push_back(rabbit9);
	m_gameObjects.push_back(bin);

	// ===== Lighting ===== //


	Vector3 light_pos(500,500,-100);
	Vector4 light_col(1,1,1,1);
	//float	light_rad = 3000;

	Vector3 light_scale(3000, 3000, 3000);

	GameLight* light = new GameLight(light_pos, light_col, light_scale);
	light->setTag(T_PointLight);
	m_gameObjects.push_back(light);

	// ===== Terrain ===== //


	GameHeightMap* terrain = new GameHeightMap(Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(10, 10, 10));
	terrain->setTag(T_Terrain);
	m_gameObjects.push_back(terrain);

	// ===== Water ===== //

	GameHeightMap* water = new GameHeightMap(Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(10, 10, 10));
	water->setTag(T_Water);
	m_gameObjects.push_back(water);


	

}
