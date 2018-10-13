////========================================
// Class: EventNode
// Author: Hemmy
// Date: 12/06/2018
// Description:
//
// ========================================

#include "EventNode.h"


EventNode::EventNode(EventBus* bus, SubSystem system):m_bus(bus),m_system(system) {
	bus->addSubsystem(system, this);
	
}
