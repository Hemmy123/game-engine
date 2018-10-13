////========================================
// Class: PhysicsNode
// Author: Hemmy
// Date: 11/09/2018
// Description: Class that handles communication
// between the physics subsystem and the other
// subsystems
//
// ========================================
#pragma once
#include "EventBus.h"
#include <stdio.h>

class PhysicsNode: public EventNode{
public:
	PhysicsNode(EventBus* bus, SubSystem subSystem);
	~PhysicsNode();
	
	void handleEvent(Event event);
	void update(float dt);
	
private:
	
};


