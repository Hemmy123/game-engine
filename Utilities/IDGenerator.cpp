////========================================
// Class: IDGenerator
// Author: Hemmy
// Date: 03/06/2018
// Description:
//
// ========================================
#include <iostream>

#include "IDGenerator.h"

IDGenerator::IDGenerator(){
}
IDGenerator::~IDGenerator(){
	
}


const char*  IDGenerator::generateID(){
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	const std::string tmp = boost::lexical_cast<std::string>(uuid);
	const char* ID = tmp.c_str();
	return ID;
	
}
