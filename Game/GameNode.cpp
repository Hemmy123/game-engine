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
	m_currentLevel->createDemoLevel();

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
		m_currentLevel->createDemoLevel();


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
		m_currentLevel->createDeferredLevelDemo();


		Event graphicsEvent(Sys_Game, Sys_Graphics, "Load_Level", m_currentLevel);
		Event physicsEvent(Sys_Game, Sys_Physics, "Load_Level", m_currentLevel);

		m_bus->addEvent(graphicsEvent);
		m_bus->addEvent(physicsEvent);

	}

	
}



void GameNode::handleEvent(Event e){

	
}


void GameNode::loadTestLevel(){
	
	
}
