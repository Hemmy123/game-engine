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


double Timer::calculateFPS(double start, double end) {
	double difference = end - start;
	return 1 / difference;

}


double Timer::getDelta(){
	
	double now = glfwGetTime();
	double difference = now - m_lastRecorded ; // dt in seconds
	m_lastRecorded = glfwGetTime();
	
	return (difference * 1000); // dt in milliseconds
	
}

double Timer::getTime()
{
	return glfwGetTime() * 1000;
}



