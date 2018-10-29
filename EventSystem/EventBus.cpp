////========================================
// Class: EventBus
// Author: Hemmy
// Date: 12/06/2018
// Description:
//
// ========================================

#include "EventBus.h"
#include <iostream>

EventBus::EventBus(){
	
}

EventBus::~EventBus(){

	// TODO: Delete subsystems.
}

void EventBus::addEvent(Event event){
	m_eventQueue.push(event);
	
}


void EventBus::dispatchEvents(){
	if(!m_eventQueue.empty()){
		Event event = m_eventQueue.front();
		
		SubSystem receiver = event.getReceiver();
		auto mapIter = m_subsystems.find(receiver);
		
		if( mapIter == m_subsystems.end() ){
			// TODO: Log error using logger!
			std::cout<<"That subsystem does not exist!" <<std::endl;
			
		} else{
			mapIter->second->handleEvent(event);
		}
		
		m_eventQueue.pop();
	}
	
}

void EventBus::update() {
    dispatchEvents();

}

void EventBus::addSubsystem(SubSystem system, EventNode *node) { 
	// if subsystem does not exist (cannot be found in map)
	// Add it to the map.
	
	if(m_subsystems.find(system) == m_subsystems.end()){
		m_subsystems.insert({system,node});
	
	} else{
		//TODO: Use logger to log an error
		std::cout <<"That system already exists!" << std::endl;
	}
	
}


