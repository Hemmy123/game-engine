#pragma once
#include "GameObject.h"


/// Pretty much an exact copy of the Light class in the graphics subsystem,
/// however this is meant to be expanded later on to possibily include
/// movement or any other fancy stuff.

enum class GameLightType{
	PointLight,
	SpotLight
};

class GameLight :public GameObject{
public:
	GameLight(Vector3 position, Vector4 colour, float radius) :
		m_position(position), 
		m_colour(colour), 
		m_radius(radius) {
		m_scale = Vector3(radius, radius, radius);
		m_type = GameLightType::PointLight;
	
	};


	GameLight(Vector3 position, Vector4 colour, Vector3 scale) :
		m_position(position),
		m_colour(colour),
		m_scale(scale) {

		m_type = GameLightType::SpotLight;
		m_radius = m_scale.y;
	};


	~GameLight() {};

	// ---------- Getters / Setters ---------- //
	Vector3 getPosition() 	const { return m_position; }
	Vector4 getColour() 	const { return m_colour; }
	Vector3 getRadius()		const { return m_scale; }
	GameLightType getType() const { return m_type; }

	void setPosition(Vector3 position)	{ m_position = position; }
	void setColour(Vector4 colour)		{ m_colour	= colour; }
	void setScale(Vector3 scale)		{ m_scale	= scale; }
	void setRadius(float radius)		{ m_radius	= radius; }
	void setType(GameLightType type)	{ m_type = type; }
private:
	GameLightType m_type;
	Vector3 m_position;
	Vector4 m_colour;
	float	m_radius;
	Vector3	m_scale;
};

