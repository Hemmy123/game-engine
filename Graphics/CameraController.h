#pragma once
#include "Camera.h"
#include <vector>
class CameraController
{
public:
	CameraController(Camera* camera);
	~CameraController();


	void update(float msec);

	void setPaused(bool paused) { m_paused = paused; }
	bool getPaused()	const { return m_paused; }


	void setCurrentScene(int scene) { m_currentScene = scene; }
	int getCurrentScene()	const { return m_currentScene; }


	void setSpeed(int speed)	{ m_speed = speed; }
	float getSpeed()			const { return m_speed; }

	void setPositions(std::vector<Vector3> positions) { m_position = positions; }

	Vector3 getNormalisedDirectionalVector(const Vector3& from, const Vector3& to);

	// Scene Specfic functions

	void createScene1();
	void createScene2();
	void createScene3();

private:

	/// Is the scene paused
	bool	m_paused;

	/// Has the camrea reached it's next position?
	bool m_reachedPosition;

	/// the ID of the current scene
	int		m_currentScene;

	/// The speed the camera should move at
	float	m_speed;

	/// the set of positions to go to in the current scene
	std::vector<Vector3> m_position;
	
	
	Camera*	m_camera;
};

