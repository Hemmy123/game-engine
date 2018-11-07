#include "Shadows.h"



Shadows::Shadows()
{
	m_sceneShader = new Shader(
		SHADERVERTDIR"shadowScene_vert.glsl",
		SHADERFRAGDIR"shadowScene_frag.glsl");

	m_shadowShader = new Shader(
		SHADERVERTDIR"shadow_vert.glsl",
		SHADERFRAGDIR"shadow_frag.glsl");


	Vector3 pos(100, 400, 100);
	Vector4 col(1, 1, 1, 1);
	float radius = 6000.0f;
	m_light = new Light(pos, col, radius);


}


Shadows::~Shadows()
{
}
