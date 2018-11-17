//
//  Mesh.cpp
//  Graphics
//
//  Created by Hemmy on 02/06/2018.
//  Copyright © 2018 Hemmy. All rights reserved.
//

#include "Mesh.h"
#include "tiny_obj_loader.h"
//
//#include "OBJReader.h"
//#include "TinyObjConverter.h"

Mesh::Mesh(){
	m_id			= 0;
	m_texture		= 0;
	m_bumpTexture	= 0;
	m_hasBumpTexture = false;

    m_numVertices = 0;
	m_type = GL_TRIANGLES;
	m_textureType = Texture_2D;			// Default to 2D texture
    for(int i = 0; i < MAX_BUFFER; ++i) {
        m_VBO[i] = 0;
    }
	m_numIndices	= 0;
	m_vertices		= nullptr;
	m_textureCoords	= nullptr;
	m_normals		= nullptr;
	m_tangents		= nullptr;
	m_indices		= nullptr;
	m_colours		= nullptr;
}

Mesh::~Mesh(){
	

	
	glDeleteTextures(1, &m_texture);		// Deletes Texture
	glDeleteTextures(1, &m_bumpTexture);		// Deletes Texture
    glDeleteVertexArrays(1, &m_VAO);        // Delete our VAO
	glDeleteBuffers(MAX_BUFFER, m_VBO);     // Delete our VBOs
    
    //Later tutorial stuff
    delete[]m_vertices;
    delete[]m_colours;
    delete[]m_textureCoords;
    delete[]m_normals;
    delete[]m_tangents;
    delete[]m_indices;
    
}



void Mesh::generateNormals(){
	
	// Create array Vector3
	if(!m_normals){
	
		m_normals = new Vector3[m_numVertices];
	} else{
		delete[] m_normals;
		m_normals = new Vector3[m_numVertices];

	}
	// Set each value to Vector3(0,0,0)
	for(GLuint i = 0; i <m_numVertices; i++){
		m_normals[i] = Vector3();
	}
	
	// Iterate through indicies
	if(m_indices){
		for(GLuint i = 0; i< m_numIndices; i += 3){
			unsigned int a = m_indices[i];
			unsigned int b = m_indices[i + 1];
			unsigned int c = m_indices[i + 2];
			
			Vector3 normal = Vector3::Cross(m_vertices[b] - m_vertices[a],
											m_vertices[c] - m_vertices[a]);
			
			m_normals[a] += normal;
			m_normals[b] += normal;
			m_normals[c] += normal;
		}
	} else{
		
		for(GLuint i = 0; i < m_numVertices; i += 3){
			Vector3 &a = m_vertices[i];
			Vector3 &b = m_vertices[i + 1];
			Vector3 &c = m_vertices[i + 2];
			
			Vector3 normal = Vector3::Cross(b-a, c-a);
			
			m_normals[i] 	 += normal;
			m_normals[i + 1] += normal;
			m_normals[i + 2] += normal;
		}
	}
	
	// Normalise all normals
	for(GLuint i = 0; i <m_numVertices; ++i){
		m_normals[i].Normalise();
	}
	
}

void Mesh::bindTexture()
{
	switch (m_textureType) {
	case Texture_2D: {
		glActiveTexture(GL_TEXTURE0 + TextureUniforms::Diffuse);
		glBindTexture(GL_TEXTURE_2D, m_texture); 
		
		if (m_hasBumpTexture) {
			glActiveTexture(GL_TEXTURE0 + TextureUniforms::Bump);
			glBindTexture(GL_TEXTURE_2D, m_bumpTexture);
		}

		break; 
	}

	case Cube_Map : glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture); break;
	default : glBindTexture(GL_TEXTURE_2D, m_texture); break;
	}


}

void Mesh::generateTangents(){
	if (!m_tangents) {
		m_tangents = new  Vector3[m_numVertices];

	}
	if (!m_textureCoords) {
		return; //Can’t use  tex  coords  if  there  aren’t any!

	}
	for (GLuint i = 0; i < m_numVertices; ++i) {
		m_tangents[i] = Vector3();

	}

	if (m_indices) {
		for (GLuint i = 0; i < m_numIndices; i += 3) {
			int a = m_indices[i];
			int b = m_indices[i + 1];
			int c = m_indices[i + 2];

			Vector3  tangent = generateTangent(
				m_vertices[a],		m_vertices[b],
				m_vertices[c],		m_textureCoords[a],
				m_textureCoords[b], m_textureCoords[c]);

			m_tangents[a] += tangent;
			m_tangents[b] += tangent;
			m_tangents[c] += tangent;

		}

	}
	else {
		for (GLuint i = 0; i < m_numVertices; i += 3) {
			Vector3  tangent = generateTangent(
				m_vertices[i],		m_vertices[i + 1],		m_vertices[i + 2], 
				m_textureCoords[i],	m_textureCoords[i + 1], m_textureCoords[i + 2]);

			m_tangents[i] += tangent;
			m_tangents[i + 1] += tangent;
			m_tangents[i + 2] += tangent;

		}

	}
	for (GLuint i = 0; i < m_numVertices; ++i) {
		m_tangents[i].Normalise();

	}

}

Vector3 Mesh::generateTangent(const Vector3 & a, const Vector3 & b, const Vector3 & c, const Vector2 & ta, const Vector2 & tb, const Vector2 & tc)
{
	Vector2  coord1 = tb - ta;
	Vector2  coord2 = tc - ta;

	Vector3  vertex1 = b - a;
	Vector3  vertex2 = c - a;

	Vector3  axis = Vector3(vertex1*coord2.y - vertex2 * coord1.y);

	float  factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);

	return  axis * factor;
}




Mesh* Mesh::generateQuad(){
	
	Mesh* m = new Mesh();
	m->m_type = GL_TRIANGLES;

	m->m_numVertices = 6;
	
	m->m_vertices = new Vector3[m->m_numVertices];
	
	m->m_vertices[0] = 	Vector3(-1.0f, -1.0f, 0.0f);
	m->m_vertices[1] = 	Vector3(1.0f,	-1.0f, 0.0f);
	m->m_vertices[2] = 	Vector3(-1.0f, 1.0f, 0.0f);
	
	m->m_vertices[3] = 	Vector3(1.0f,	-1.0f, 0.0f);
	m->m_vertices[4] = 	Vector3(1.0f, 1.0f, 0.0f);
	m->m_vertices[5] = 	Vector3(-1.0f,  1.0f, 0.0f);


	
	m->m_textureCoords = new Vector2[m->m_numVertices];

	m->m_textureCoords[0] = Vector2(0.0f,	0.0f);
	m->m_textureCoords[1] = Vector2(1.0f,	0.0f);
	m->m_textureCoords[2] = Vector2(0.0f,	1.0f);
	
	m->m_textureCoords[3] = Vector2(1.0f,	0.0f);
	m->m_textureCoords[4] = Vector2(1.0f,	1.0f);
	m->m_textureCoords[5] = Vector2(0.0f,	1.0f);
	//
	//
	//m->m_textureCoords[0] = Vector2(0.0f, 1.0f);
	//m->m_textureCoords[1] = Vector2(1.0f, 1.0f);
	//m->m_textureCoords[2] = Vector2(0.0f, 0.0f);

	//m->m_textureCoords[3] = Vector2(1.0f, 1.0f);
	//m->m_textureCoords[4] = Vector2(1.0f, 0.0f);
	//m->m_textureCoords[5] = Vector2(0.0f, 0.0f);


	return m;
	
	
}


Mesh* Mesh::generateTriangle(){
    Mesh* m = new Mesh();
	m->m_numVertices = 3;

	m->m_vertices = new Vector3[m->m_numVertices];
	m->m_vertices[0] = Vector3(0.0f, 0.5f, 0.0f);
	m->m_vertices[1] = Vector3(0.5f, -0.5f, 0.0f);
	m->m_vertices[2] = Vector3(-0.5f, -0.5f, 0.0f);

	m->m_textureCoords = new Vector2[m->m_numVertices];
	m->m_textureCoords[0] = Vector2(0.5f, 0.0f);
	m->m_textureCoords[1] = Vector2(1.0f, 1.0f);
	m->m_textureCoords[2] = Vector2(0.0f, 1.0f);

	m->m_colours = new Vector4[m->m_numVertices];
	m->m_colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	m->m_colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	m->m_colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	return m;
}


// TODO: Clean this method up! It was left so I could quickly test
// lighting and other stuff
Mesh* Mesh::readObjFile(std::string path){
	
	tinyobj::attrib_t 					attrib;
	std::vector<tinyobj::shape_t> 		shapes;
	std::vector<tinyobj::material_t> 	materials;
	std::string error;
	
	
	
	std::vector<Vector3> vertices;
	std::vector<Vector2> texCoords;
	std::vector<Vector3> normals;
	
	
	bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &error, path.c_str(),
								NULL, false);
	
	if (!error.empty()) {
		std::cerr << error << std::endl;
	}
	
	if (!result) {
		printf("Failed to load/parse .obj.\n");
		//return false;
	}
	
	
	// Loop over shapes
	for( int s = 0; s < shapes.size(); s++){
		int index_offset = 0;
		
		// Loop over faces
		for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			
			// should always be 3 since triangles
			int fv = shapes[s].mesh.num_face_vertices[f];
			

			// Loop over vertices in that face
			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				
				tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
				tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
				tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
				
				Vector3 tempV(vx,vy,vz);
				vertices.push_back(tempV);
				
				tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
				tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
				tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
				
				Vector3 tempN(nx,ny,nz);
				normals.push_back(tempN);
				
				tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
				tinyobj::real_t ty = 1 - attrib.texcoords[2*idx.texcoord_index+1];
				
				Vector2 tempT(tx,ty);
				texCoords.push_back(tempT);
				
			}
			index_offset += fv;
			
			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
	
	Mesh* m = new Mesh();

	m->m_numVertices = vertices.size();

	m->m_vertices = new Vector3[vertices.size()];
	for(int i = 0 ; i < vertices.size() ; i++){
		m->m_vertices[i] = vertices[i];
	}

	m->m_textureCoords = new Vector2[texCoords.size()];
	for(int i = 0 ; i < texCoords.size() ; i++){
		m->m_textureCoords[i] = texCoords[i];
	}

	m->m_normals = new Vector3[normals.size()];
	for(int i = 0 ; i < normals.size(); i++){
		m->m_normals[i] = normals[i];
	}



	
	
	return m;
}

void Mesh::updateData(){
	// update to use map instead?

	glBindVertexArray(m_VAO);
	
	//Buffer vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector3), m_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_BUFFER);
	
	//Buffer texture data
	if (m_textureCoords) {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector2), m_textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}
	
	//buffer colour data
	if (m_colours) {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector4), m_colours, GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}
	
	//buffer index data
	if (m_indices) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), m_indices, GL_STATIC_DRAW);
	}
	
	if(m_normals){
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector3), m_normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0,0);
		glEnableVertexAttribArray(NORMAL_BUFFER);
	}
	
	//Once we're done with the vertex buffer binding, we can unbind the VAO,
	//ready to reapply later, such as in the Draw function above!
	glBindVertexArray(0);
	
}

void Mesh::bufferData(){
    glGenVertexArrays(1, &m_VAO);
    
    glBindVertexArray(m_VAO);
    
    //Buffer vertex data
    glGenBuffers(1, &m_VBO[VERTEX_BUFFER]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector3), m_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(VERTEX_BUFFER);


    
    //Buffer texture data
    if (m_textureCoords) {
        glGenBuffers(1, &m_VBO[TEXTURE_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TEXTURE_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector2), m_textureCoords, GL_STATIC_DRAW);
        glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(TEXTURE_BUFFER);
    }

	if (m_tangents) {
		glGenBuffers(1, &m_VBO[TANGENT_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TANGENT_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector3),
			m_tangents,GL_STATIC_DRAW);
		glVertexAttribPointer(TANGENT_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TANGENT_BUFFER);

	}
    
    //buffer colour data
    if (m_colours) {
        glGenBuffers(1, &m_VBO[COLOUR_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[COLOUR_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector4), m_colours, GL_STATIC_DRAW);
        glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(COLOUR_BUFFER);
    }
    
    //buffer index data
    if (m_indices) {
        glGenBuffers(1, &m_VBO[INDEX_BUFFER]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[INDEX_BUFFER]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), m_indices, GL_STATIC_DRAW);
    }
	
	if(m_normals){
		glGenBuffers(1, &m_VBO[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector3), m_normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0,0);
		glEnableVertexAttribArray(NORMAL_BUFFER);
	}
	
    //Once we're done with the vertex buffer binding, we can unbind the VAO,
    //ready to reapply later, such as in the Draw function above!
    glBindVertexArray(0);
}

void Mesh::draw(){
    
	//glBindTexture(GL_TEXTURE_2D, m_texture);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

	
    glBindVertexArray(m_VAO);
	

    // If we're using indices:
    if(m_VBO[INDEX_BUFFER]) {
        glDrawElements(m_type, m_numIndices, GL_UNSIGNED_INT, 0);
    }
    else{
        // Else just use vertices
        glDrawArrays(m_type, 0, m_numVertices);    //Draw the triangle!
	
		int i = 0;
    }
    // Unbind for good practice
    glBindVertexArray(0);
	
	glBindTexture(GL_TEXTURE_2D, 0);

}





