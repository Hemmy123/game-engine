////========================================
// Class: Event
// Author: Hemmy
// Date: 12/06/2018
// Description:
//
// ========================================

#pragma once

#include <stdio.h>
#include <string>

enum SubSystem{
	Sys_Graphics,
	Sys_Game,
	Sys_Physics,
	Sys_Sounds,
	Sys_IO,
	Sys_Interface
};


class Event{
	
public:
	Event(SubSystem sender, SubSystem receiver, std::string type, void* data = nullptr){
		m_sender = sender;
		m_receiver = receiver;
		m_type = type;
		m_data = data;
	}
	~Event(){};
	
	// Copy Constructor
	Event(const Event &rhs){
		m_sender = rhs.m_sender;
		m_receiver = rhs.m_receiver;
		m_type = rhs.m_type;
		m_data = rhs.m_data; // is this correct?
	}
	
	
	
	SubSystem getSender() 	const { return m_sender;}
	SubSystem getReceiver() const { return m_receiver;}
	std::string getType() 	const { return m_type;}
	void* getData()			const { return m_data; }


	
private:
	SubSystem m_sender;
	SubSystem m_receiver;
	std::string m_type;
	// Pointer to some user defined data, never delete in
	// this class as someone else controls it!
	void* m_data;

	
};


