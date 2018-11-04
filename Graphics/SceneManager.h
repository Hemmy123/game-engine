#pragma once
#include "RenderObject.h"

using std::vector;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();



	vector<RenderObject*> getTransparent()  { return m_transparent; };
	vector<RenderObject*> getOpaque()  { return m_opaque; };

	void pushRenderObject(RenderObject* ro);

private:
	vector<RenderObject*> m_transparent;
	vector<RenderObject*> m_opaque;
};

