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
	loadTestLevel();


	
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
	

	if (KeyboardInterface::m_heldKeys[GLFW_KEY_1]) {
		std::cout << "1 Helded" << std::endl;

	}


	if (KeyboardInterface::m_pressedKeys[GLFW_KEY_2]) {
		std::cout << "2 Pressed" << std::endl;
	}

	//switch(s.m_key){
	//case(GLFW_KEY_ESCAPE): {
	//	m_endGame = true;
	//	break;
	//}
	//case(GLFW_KEY_1): {
	//	std::cout << "1 Pressed!" << std::endl;

	//	/*std::cout << "1 pressed" << std::endl;
	//	if (!m_currentLevel) {
	//		m_currentLevel = new Level();
	//	}
	//	else {
	//		delete m_currentLevel;
	//		m_currentLevel = new Level();
	//	}
	//	m_currentLevel->createDemoLevel();


	//	Event graphicsEvent(Sys_Game, Sys_Graphics, "Load_Level", m_currentLevel);
	//	Event physicsEvent(Sys_Game, Sys_Physics, "Load_Level", m_currentLevel);

	//	m_bus->addEvent(graphicsEvent);
	//	m_bus->addEvent(physicsEvent);*/
	//	break;
	//}
	//case(GLFW_KEY_2): {
	///*	if (!m_currentLevel) {
	//		m_currentLevel = new Level();
	//	}
	//	else {
	//		delete m_currentLevel;
	//		m_currentLevel = new Level();
	//	}
	//	m_currentLevel->createDeferredLevelDemo();


	//	Event graphicsEvent(Sys_Game, Sys_Graphics, "Load_Level", m_currentLevel);
	//	Event physicsEvent(Sys_Game, Sys_Physics, "Load_Level", m_currentLevel);

	//	m_bus->addEvent(graphicsEvent);
	//	m_bus->addEvent(physicsEvent);*/
	//	break;
	//}

	//}

	
}



void GameNode::handleEvent(Event e){

	
}


void GameNode::loadTestLevel(){
	
	
}
