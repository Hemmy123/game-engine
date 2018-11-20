#include "GraphicsScene.h"
#include "FilePaths.h"
GraphicsScene::GraphicsScene()
{
}

GraphicsScene::~GraphicsScene()
{
}


void GraphicsScene::loadScene1()
{
	string lightingVert = SHADERVERTDIR"ShadowScene_Vert.glsl";
	string lightingFrag = SHADERFRAGDIR"ShadowScene_Frag.glsl";
	Shader* shader = new Shader(lightingVert, lightingFrag);
	Shader* transShader = new Shader(lightingVert, lightingFrag);




}

void GraphicsScene::loadScene2()
{
}

void GraphicsScene::loadScene3()
{
}

RenderObject * GraphicsScene::getScene(int i)
{

	switch (i) {
	case 1: return m_scene1; break;
	case 2: return m_scene2; break;
	case 3: return m_scene3; break;
	default: return m_scene1;
	}

}
