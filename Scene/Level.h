////========================================
// Class: Level
// Author: Hemmy
// Date: 11/07/2018
// Description:
// A class that holds a bunch of gameobjects
// that are to be loaded in my the graphics
// and physics subsystems
// ========================================

#ifndef Level_hpp
#define Level_hpp

#include <stdio.h>
#include <vector>
#include "GameObject.h"

class Level {
public:
	
	Level();
	~Level();
	
	std::vector<GameObject*> getGameObjects() { return m_gameObjects;}
	
	void createDemoLevel();
	
private:
	std::vector<GameObject*> m_gameObjects;
};

#endif /* Level_hpp */
