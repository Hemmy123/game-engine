////========================================
// Class: GameNode
// Author: Hemmy
// Date: 14/06/2018
// Description:
//
// ========================================

#pragma once

#include <stdio.h>
#include "EventBus.h"
#include "InterfaceHandler.h"
#include "Level.h"

class GameNode:public EventNode {
public:
	GameNode(EventBus* bus, SubSystem subSystem, InterfaceHandler* ih);
	~GameNode();
	void handleEvent(Event event);
	void update(float dt);
	void checkInputs();
	
	bool getEndGame(){return m_endGame;}
	
	void cleanLevel();
	/* Test Methods */
	void loadTestLevel();
	
private:
	bool m_endGame;
	float m_dt;
	
	int m_currentLevelID;
	Level* m_currentLevel;
	InterfaceHandler* m_interfaceHandler;
	
};

