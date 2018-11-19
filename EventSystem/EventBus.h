////========================================
// Class: EventBus
// Author: Hemmy
// Date: 12/06/2018
// Description:
//
// ========================================

#pragma once

#include <stdio.h>
#include <queue>
#include <map>
#include "Event.h"
#include "EventNode.h"


// NOTE: Subsystems are linked to the bus, not the other way around
// like the in old system! (So subsystems do not contain a reference
// the main event bus. The main event bus however contains a reference
// to all of the other sub systems).
class EventNode;

class EventBus{
public:
	EventBus();
	~EventBus();
	
	// Adds a subsystem to the map of nodes the EventBus can send
	// messages to.
	void addSubsystem(SubSystem system, EventNode* node);
	void addEvent(Event event);
	void update();
	
protected:
	
	void dispatchEvents();
	
	std::queue<Event> m_eventQueue;
	std::map<SubSystem, EventNode*> m_subsystems;
};


