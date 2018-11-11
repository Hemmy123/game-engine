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
}

void Level::createDemoLevel() {

	// Hard coding a level for coursework. Should really be data driven later


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

	rabbit1->setTag(T_Rabbit);
	rabbit2->setTag(T_Rabbit);
	rabbit3->setTag(T_Rabbit);
	rabbit4->setTag(T_Rabbit);
	rabbit5->setTag(T_Rabbit);
	rabbit6->setTag(T_Rabbit);
	rabbit7->setTag(T_Rabbit);
	rabbit8->setTag(T_Rabbit);
	rabbit9->setTag(T_Rabbit);

	m_gameObjects.push_back(rabbit1);
	m_gameObjects.push_back(rabbit2);
	m_gameObjects.push_back(rabbit3);
	m_gameObjects.push_back(rabbit4);
	m_gameObjects.push_back(rabbit5);
	m_gameObjects.push_back(rabbit6);
	m_gameObjects.push_back(rabbit7);
	m_gameObjects.push_back(rabbit8);
	m_gameObjects.push_back(rabbit9);

	// ===== Lighting ===== //


	Vector3 light_pos(500,500,-300);
	Vector4 light_col(1,1,1,1);
	float	light_rad = 1000;

	GameLight* light = new GameLight(light_pos, light_col, light_rad);
	light->setTag(T_Light);
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
