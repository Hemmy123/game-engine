#pragma once
#include "Camera.h"
#include <vector>
class CameraController
{
public:
	CameraController(Camera* camera);
	~CameraController();


	void update(float msec);



	void moveCamera(Vector3 towards);

	bool withinRadius(Vector3 a, Vector3 b);

	// -- Getters/Setters -- //
	void setPaused(bool paused);
	bool getPaused()	const { return m_paused; }


	void setCurrentScene(int scene) { m_currentScene = scene; }
	int getCurrentScene()	const { return m_currentScene; }


	void setSpeed(int speed)	{ m_speed = speed; }
	float getSpeed()			const { return m_speed; }

	void setPositions(std::vector<Vector3> positions) { m_positions = positions; }

	void setCameraPosition(Vector3 pos) { m_camera->setPosition(pos); }

	Vector3 getNormalisedDirectionalVector(const Vector3& from, const Vector3& to);

	// --- Scene Specfic functions --- //

	void loadScenePositions(int scene);

	void createScene1();
	void createScene2();
	void createScene3();

private:

	float m_dt;

	/// Is the scene paused
	bool m_paused;

	/// Has the camrea reached it's next position?
	bool m_reachedPosition;

	/// the area the camera has to enter to count as 'reached'
	bool m_positionRadius;

	/// the ID of the current scene
	int	m_currentScene;

	/// The index of the current vector 3 the camera is trying to head to
	int m_currentPositionIndex;
	
	/// The speed the camera should move at
	float	m_speed;

	/// the set of positions to go to in the current scene
	std::vector<Vector3> m_positions;
	
	/// Current position of the camera is saved when it is automatically
	/// moving around the scene. This is so when the user pauses, and moves
	/// around, the camera can return to this position to continue the scene. 
	Vector3 m_currentPos;

	Camera*	m_camera;
};

