//
//  Mesh.h
//  Graphics
//
//  Created by Hemmy on 02/06/2018.
//  Adapted from NCLGLs Mesh class by Rich Davison
//

#pragma once


#define GLEW_STATIC
#include <GL/glew.h>
#include <stdio.h>


#include "SOIL2.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

enum MeshBuffer {
	VERTEX_BUFFER = 0,
	COLOUR_BUFFER = 1,
	TEXTURE_BUFFER,
	NORMAL_BUFFER,
	TANGENT_BUFFER,
	INDEX_BUFFER,
	MAX_BUFFER
};

enum TextureType {
	Texture_2D,
	Cube_Map
};

/*
Stores mesh data and has to be manually buffered into OGL
with the bufferData method.
 
*/
class Mesh {
public:
    Mesh();
    ~Mesh();
    
	static Mesh* generateTriangle();
	static Mesh* generateQuad();
	static Mesh* generateWaterQuad();
	
	
	static Mesh* readObjFile(std::string path);
	
    void bufferData();
	void updateData();
	
    void draw();
	void generateNormals();
	
    GLuint m_type;

	void setTexture(GLuint tex) {m_texture = tex;}

	void bindTexture();

	GLuint getTexture() const { return m_texture; }

	GLuint loadTexture(std::string path) {
		m_texture = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
		return m_texture;
	}

	void setTextureType(TextureType type) { m_textureType = type; }
	TextureType setTextureType() const { return m_textureType; }
	
protected:
    GLuint m_VAO;
    GLuint m_VBO[MAX_BUFFER];

	GLuint m_texture;
	

	TextureType		m_textureType;


	//Number of vertices for this mesh
	GLuint			m_numVertices;
	//Number of indices for this mesh
	GLuint			m_numIndices;

	//Pointer to vertex position attribute data (badly named...?)
	Vector3*		m_vertices;
	//Pointer to vertex colour attribute data
	Vector4*		m_colours;
	//Pointer to vertex texture coordinate attribute data
	Vector2*		m_textureCoords;
	//Pointer to vertex normals attribute data
	Vector3*		m_normals;
	//Pointer to vertex tangents attribute data
	Vector3*		m_tangents;
	//Pointer to vertex indices attribute data
	unsigned int*	m_indices;

};


 /* Mesh_hpp */
