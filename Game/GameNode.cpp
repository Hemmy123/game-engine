////========================================
// Class: GameNode
// Author: Hemmy
// Date: 14/06/2018
// Description:
//
// ========================================

#include "GameNode.h"

GameNode::GameNode(EventBus* bus,SubSystem subSystem, InterfaceHandler* ih):
EventNode(bus,subSystem),
m_endGame(false),
m_interfaceHandler(ih){
	//loadTestLevel();


	m_currentLevel = new Level();
	m_currentLevel->createScene1();

	Event graphicsEvent(Sys_Game, Sys_Graphics, "Load_Level", m_currentLevel);
	Event physicsEvent(Sys_Game, Sys_Physics, "Load_Level", m_currentLevel);

	m_bus->addEvent(graphicsEvent);
	m_bus->addEvent(physicsEvent);

	
}


GameNode::~GameNode(){
	// TODO: Delete current level here?
}

void GameNode::update(float dt){
	m_dt = dt;
	checkInputs();
	
}

void GameNode::checkInputs(){
	//KeyState s = m_interfaceHandler->getKeyState();

	
	bool* pressedKeys = m_interfaceHandler->getPressedKeys();


	if(pressedKeys[GLFW_KEY_ESCAPE]) {
		m_endGame = true;
	}
	if(pressedKeys[GLFW_KEY_1]) {
		std::cout << "Level 1 loaded" << std::endl;
		if (!m_currentLevel) {
			m_currentLevel = new Level();
		}
		else {
			delete m_currentLevel;
			m_currentLevel = new Level();
		}
		m_currentLevel->createScene1();


		Event graphicsEvent(Sys_Game, Sys_Graphics, "Load_Level", m_currentLevel);
		Event physicsEvent(Sys_Game, Sys_Physics, "Load_Level", m_currentLevel);

		m_bus->addEvent(graphicsEvent);
		m_bus->addEvent(physicsEvent);
	}
	if(pressedKeys[GLFW_KEY_2]) {
		std::cout << "Level 2 loaded" << std::endl;

		if (!m_currentLevel) {
			m_currentLevel = new Level();
		}
		else {
			delete m_currentLevel;
			m_currentLevel = new Level();
		}
		m_currentLevel->createScene2();


		Event graphicsEvent(Sys_Game, Sys_Graphics, "Load_Level", m_currentLevel);
		Event physicsEvent(Sys_Game, Sys_Physics, "Load_Level", m_currentLevel);

		m_bus->addEvent(graphicsEvent);
		m_bus->addEvent(physicsEvent);

	}

	if (pressedKeys[GLFW_KEY_3]) {
		std::cout << "Level 3 loaded" << std::endl;

		if (!m_currentLevel) {
			m_currentLevel = new Level();
		}
		else {
			delete m_currentLevel;
			m_currentLevel = new Level();
		}
		m_currentLevel->createScene3();


		Event graphicsEvent(Sys_Game, Sys_Graphics, "Load_Level", m_currentLevel);
		Event physicsEvent(Sys_Game, Sys_Physics, "Load_Level", m_currentLevel);

		m_bus->addEvent(graphicsEvent);
		m_bus->addEvent(physicsEvent);

	}

	if (pressedKeys[GLFW_KEY_PAUSE]) {
		std::cout << "Paused" << std::endl;

		Event graphicsEvent(Sys_Game, Sys_Graphics, "Pause_Level");

		m_bus->addEvent(graphicsEvent);
	}

	if (pressedKeys[GLFW_KEY_KP_1]) {
		Event graphicsEvent(Sys_Game, Sys_Graphics, "Load_Effect_1");
		m_bus->addEvent(graphicsEvent);
	}

	if (pressedKeys[GLFW_KEY_KP_2]) {
		Event graphicsEvent(Sys_Game, Sys_Graphics, "Load_Effect_2");
		m_bus->addEvent(graphicsEvent);
	}

	if (pressedKeys[GLFW_KEY_KP_3]) {
		Event graphicsEvent(Sys_Game, Sys_Graphics, "Load_Effect_3");
		m_bus->addEvent(graphicsEvent);
	}

	if (pressedKeys[GLFW_KEY_KP_4]) {
		Event graphicsEvent(Sys_Game, Sys_Graphics, "Load_Effect_4");
		m_bus->addEvent(graphicsEvent);
	}




	
}



void GameNode::handleEvent(Event e){

	
}


void GameNode::loadTestLevel(){
	
	
}
