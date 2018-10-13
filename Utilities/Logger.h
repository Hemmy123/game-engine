////========================================
// Class: Logger
// Author: Hemmy
// Date: 14/06/2018
// Description:
//
// ========================================

#pragma once


#include <stdio.h>
#include <string>

enum LogType{
	Error,
	Info
};

enum LogPriority {
	High,
	Medium,
	Low,
};

class Logger {
public:
	Logger();
	~Logger();
	
	void logEvent(LogType type, std::string event);
	
};

