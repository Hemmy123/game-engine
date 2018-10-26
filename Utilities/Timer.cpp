////========================================
// Class: Timer
// Author: Hemmy
// Date: 15/06/2018
// Description:
//
// ========================================

#include "Timer.h"



Timer::Timer(){
	m_lastRecorded 	= glfwGetTime();
	m_deltaTime 	= 0;
	
}

Timer::~Timer(){
	

}

float Timer::getDelta(){
	
	double now = glfwGetTime();
	double duration = now - m_lastRecorded ; // dt in seconds
	m_lastRecorded = glfwGetTime();
	
	return (duration * 1000); // dt in milliseconds
	
}

float Timer::getTime()
{
	return glfwGetTime();

}



