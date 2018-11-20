#include "CameraController.h"

CameraController::CameraController(Camera * camera):
	m_camera(camera){

}

CameraController::~CameraController()
{
}

void CameraController::update(float msec)
{
	if(!m_paused){
	
	}
}

Vector3 CameraController::getNormalisedDirectionalVector(const Vector3 & from, const Vector3 & to)
{
	Vector3 temp = to - from;
	return temp.Normalise();
}

void CameraController::createScene1()
{

	Vector3 start(0, 0, 0);
	Vector3 end(1000, 1000, -500);
}

void CameraController::createScene2()
{
}

void CameraController::createScene3()
{
}
