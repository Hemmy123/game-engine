//
//  Shader.h
//  Graphics
//
//  Created by Hemmy on 31/05/2018.
//

#pragma once

#define GLEW_STATIC
#include <GL/glew.h>



#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Light.h"
/**
IO code taken from:
https://badvertex.com/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c.html
*/


enum ShaderStage {
	SHADER_VERTEX = 0,
	SHADER_FRAGMENT,
	SHADER_GEOMETRY,
	SHADER_TCS,
	SHADER_TES,
	SHADER_MAX
};


using std::string;
class Shader{
public:
    
    Shader(string vertex_path, string fragment_path);
    ~Shader();
    
    string readFile(const char* filePath);
    GLuint loadVertexAndFrag(const char *vertex_path, const char *fragment_path);
    
	bool linkProgram();
    
    void checkShader(GLuint shader);
	void bindAttributes();
    
    GLuint getProgram() {return m_program;};
	
	bool getLinkSuccess()	const{return m_linkSuccess;}

	void setId(int i) { m_id = i; }
	int getId() const { return m_id; }

private:
	int		m_id;
	bool	m_linkSuccess;
    GLuint 	m_program;
    
};
 

