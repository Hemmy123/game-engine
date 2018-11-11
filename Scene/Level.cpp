////========================================
// Class: Level
// Author: Hemmy
// Date: 11/07/2018
// Description:
//
// ========================================

#include "Level.h"
#include "GameHeightMap.h"


Level::Level() {

}

Level::~Level() {

}

void Level::createDemoLevel() {

	Vector3 scale(30, 30, 30);

	int offSet = 35;
	int y = 195;
	int x = 200;
	int z = 300; 

	// Very brute for way of adding gameobjects, only for testing atm
	GameObject* rabbit1 = new GameObject(Vector3(x + -offSet * 2, y, z), Vector3(1, 1, 1), scale);
	GameObject* rabbit2 = new GameObject(Vector3(x + -offSet, y, z), Vector3(1, 1, 1), scale);
	GameObject* rabbit3 = new GameObject(Vector3(x + 0, y, z), Vector3(1, 1, 1), scale);

	GameObject* rabbit4 = new GameObject(Vector3(x + -offSet * 2, y, z + offSet), Vector3(1, 1, 1), scale);
	GameObject* rabbit5 = new GameObject(Vector3(x + -offSet, y, z + offSet), Vector3(1, 1, 1), scale);
	GameObject* rabbit6 = new GameObject(Vector3(x + 0, y, z + offSet), Vector3(1, 1, 1), scale);

	GameObject* rabbit7 = new GameObject(Vector3(x + -offSet * 2, y, z + (2 * offSet)), Vector3(1, 1, 1), scale);
	GameObject* rabbit8 = new GameObject(Vector3(x + -offSet, y, z + (2 * offSet)), Vector3(1, 1, 1), scale);
	GameObject* rabbit9 = new GameObject(Vector3(x + 0, y, z + (2 * offSet)), Vector3(1, 1, 1), scale);



	rabbit1->setTag(T_Rabbit);
	rabbit2->setTag(T_Rabbit);
	rabbit3->setTag(T_Rabbit);
	rabbit4->setTag(T_Rabbit);
	rabbit5->setTag(T_Rabbit);
	rabbit6->setTag(T_Rabbit);
	rabbit7->setTag(T_Rabbit);
	rabbit8->setTag(T_Rabbit);
	rabbit9->setTag(T_Rabbit);

	GameHeightMap* terrain = new GameHeightMap(Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(10, 10, 10));
	GameHeightMap* water = new GameHeightMap(Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(10, 10, 10));
	terrain->setTag(T_Terrain);
	water->setTag(T_Water);
	m_gameObjects.push_back(rabbit1);
	m_gameObjects.push_back(rabbit2);
	m_gameObjects.push_back(rabbit3);
	m_gameObjects.push_back(rabbit4);
	m_gameObjects.push_back(rabbit5);
	m_gameObjects.push_back(rabbit6);
	m_gameObjects.push_back(rabbit7);
	m_gameObjects.push_back(rabbit8);
	m_gameObjects.push_back(rabbit9);
	m_gameObjects.push_back(terrain);
	m_gameObjects.push_back(water);

}
