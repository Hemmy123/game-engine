#include "CameraController.h"

CameraController::CameraController(Camera * camera):
	m_camera(camera){

	m_currentPositionIndex = 0;
	m_positionRadius = 5;
}

CameraController::~CameraController()
{
}

void CameraController::update(float msec)
{
	if(!m_paused && (m_currentPositionIndex < m_positions.size()) ){

		Vector3 cameraPos = m_camera->GetPosition();
		Vector3 movingTowards = m_positions[m_currentPositionIndex];

		// If it's the first position in the vector
		if (m_currentPositionIndex = 0) {
			
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
	}
}

void CameraController::moveCamera(Vector3 towards)
{
	if (!m_reachedPosition) {
		Vector3 currentPos = m_camera->GetPosition();

		Vector3 direction = getNormalisedDirectionalVector(currentPos, towards);
		Vector3 newPos = direction * (m_dt * m_speed);
		m_camera->setPosition(newPos);
	}

}

bool CameraController::withinRadius(Vector3 a, Vector3 b)
{
	int length = (a - b).Length();
	return (length < m_positionRadius);
}

Vector3 CameraController::getNormalisedDirectionalVector(const Vector3 & from, const Vector3 & to)
{
	Vector3 temp = to - from;
	return temp.Normalise();
}

void CameraController::loadScenePositions(int scene)
{
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

}

void CameraController::createScene2()
{
}

void CameraController::createScene3()
{
}
