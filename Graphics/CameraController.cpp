#include "CameraController.h"

CameraController::CameraController(Camera * camera):
	m_camera(camera){

	m_currentPositionIndex = 0;
	m_positionRadius = 5;

	m_positions.reserve(50);
	m_speed = 0.1;
}

CameraController::~CameraController()
{
}

void CameraController::update(float msec)
{

	m_dt = msec;
	if(!m_paused && (m_currentPositionIndex < m_positions.size()) && m_camera){

		Vector3 cameraPos = m_camera->GetPosition();
		Vector3 movingTowards = m_positions[m_currentPositionIndex];

		// If it's the first position in the vector
		if (m_currentPositionIndex == 0) {
			
			// Just set the camera position instead of moving towards it
			setCameraPosition(m_positions[m_currentPositionIndex]);
			
			// Let the controller know where the camera is at
			m_currentPos = cameraPos;

			// On to the next position
			m_currentPositionIndex++;
		}
		else {

			// Is the camera within the radius of the position
			// it's moving towards to?
			if (withinRadius(cameraPos, movingTowards)) {
				m_currentPositionIndex++;
			}
			else {
				// move camera towards new position
				moveCamera(m_positions[m_currentPositionIndex]);
			}
		}
	} else {
		m_reachedPosition = true;
	}
}

void CameraController::moveCamera(Vector3 towards)
{
	
		Vector3 currentPos = m_camera->GetPosition();

		Vector3 direction = getNormalisedDirectionalVector(currentPos, towards);
		Vector3 directionalVec = direction * (m_dt * m_speed);

		Vector3 newPos = currentPos + directionalVec;
		m_currentPos = newPos;
		m_camera->setPosition(newPos);
	

}

bool CameraController::withinRadius(Vector3 a, Vector3 b)
{
	int length = (a - b).Length();
	return (length < m_positionRadius);
}

void CameraController::setPaused(bool paused)
{
	m_paused = paused;
	if (paused) {
		m_camera->setMoveable(true);
	}
	else {

		m_camera->setPosition(m_currentPos);
		m_camera->setMoveable(false);
	}
}

Vector3 CameraController::getNormalisedDirectionalVector(const Vector3 & from, const Vector3 & to)
{
	Vector3 temp = to - from;
	return temp.Normalise();
}

void CameraController::loadScenePositions(int scene)
{

	m_currentPositionIndex = 0;
	switch (scene) {
	case 1: createScene1(); break;
	case 2: createScene2(); break;
	case 3: createScene3(); break;
	default: {
		std::cout << "Camera controller could not load positions, that level does not exist!" <<std::endl;
		break;
	}
	}
}

void CameraController::createScene1()
{
	m_positions.clear();
	m_positions.push_back(Vector3(0, 0, 0));
	m_positions.push_back(Vector3(1000, 1000, -500));
	m_positions.push_back(Vector3(2000, 1000, -500));
	m_positions.push_back(Vector3(1000, 1000, -500));

}

void CameraController::createScene2()
{
	m_positions.clear();

}

void CameraController::createScene3()
{
	m_positions.clear();

}
