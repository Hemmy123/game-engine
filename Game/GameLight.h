#pragma once
#include "GameObject.h"


/// Pretty much an exact copy of the Light class in the graphics subsystem,
/// however this is meant to be expanded later on to possibily include
/// movement or any other fancy stuff.

class GameLight :public GameObject{
public:
	GameLight(Vector3 position, Vector4 colour, float radius) :m_position(position), m_colour(colour), m_radius(radius) {};
	~GameLight() {};

	// ---------- Getters / Setters ---------- //
	Vector3 getPosition() 	const { return m_position; }
	Vector4 getColour() 	const { return m_colour; }
	float 	getRadius() 	const { return m_radius; }

	void setPosition(Vector3 position) { m_position = position; }
	void setColour(Vector4 colour) { m_colour = colour; }
	void setRadius(float radius) { m_radius = radius; }

private:
	Vector3 m_position;
	Vector4 m_colour;
	float	m_radius;
};

