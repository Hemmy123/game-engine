////========================================
// Class: EventNode
// Author: Hemmy
// Date: 12/06/2018
// Description:
//
// ========================================
#pragma once


#include <stdio.h>
#include "EventBus.h"

class EventBus;

class EventNode {
public:
	EventNode(EventBus* eventBus,SubSystem system);
		
	virtual void handleEvent(Event event) = 0;
	
protected:
	EventBus * m_bus;
	SubSystem m_system;
};

