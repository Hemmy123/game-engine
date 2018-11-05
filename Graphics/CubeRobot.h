#include "RenderObject.h"

class CubeRobot : public RenderObject
{
public:
	CubeRobot();
	~CubeRobot();

	virtual void update(float msec);

	static void createCube();
	static void deleteCube() {delete m_cube;}
private:
	static Mesh* m_cube;
	RenderObject* m_head;
	RenderObject* m_leftArm;
	RenderObject* m_rightArm;


};