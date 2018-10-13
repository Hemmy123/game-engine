////========================================
// Class: IDGenerator
// Author: Hemmy
// Date: 03/06/2018
// Description:
//
// ========================================

#pragma once


#include <stdio.h>


#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>


class IDGenerator{
public:
	IDGenerator();
	~IDGenerator();
	
	static const char* generateID();
	
private:
};
