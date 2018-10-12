////========================================
// Class: IDGenerator
// Author: Hemmy
// Date: 03/06/2018
// Description:
//
// ========================================

#ifndef IDGenerator_hpp
#define IDGenerator_hpp

#include <stdio.h>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>

class IDGenerator{
public:
	IDGenerator();
	~IDGenerator();
	
	static const char* generateID();
	
private:
};
#endif /* IDGenerator_hpp */
