////========================================
// Class: Level
// Author: Hemmy
// Date: 11/07/2018
// Description:
//
// ========================================

#include "Level.hpp"


Level::Level(){
	
}

Level::~Level(){
	
}

void Level::createDemoLevel(){
	
	// Very brute for way of adding gameobjects, only for testing atm
	GameObject* rabbit1 = new GameObject(Vector3(-10,0,0), Vector3(1,1,1), Vector3(1,1,1));
	GameObject* rabbit2 = new GameObject(Vector3(-5,0,0), Vector3(1,1,1), Vector3(1,1,1));
	GameObject* rabbit3 = new GameObject(Vector3(0,0,0), Vector3(1,1,1), Vector3(1,1,1));
	GameObject* rabbit4 = new GameObject(Vector3(5,0,0), Vector3(1,1,1), Vector3(1,1,1));
	
	rabbit1->setTag(T_Rabbit);
	rabbit2->setTag(T_Rabbit);
	rabbit3->setTag(T_Rabbit);
	rabbit4->setTag(T_Rabbit);
	
	
	m_gameObjects.push_back(rabbit1);
	m_gameObjects.push_back(rabbit2);
	m_gameObjects.push_back(rabbit3);
	m_gameObjects.push_back(rabbit4);
	
}
