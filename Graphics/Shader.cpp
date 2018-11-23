//
//  Shader.cpp
//  Graphics
//
//  Created by Hemmy on 31/05/2018.
//  Copyright © 2018 Hemmy. All rights reserved.
//

#include "Shader.h"
#include "Mesh.h"

Shader::Shader(string vertex_path, string fragment_path){
	m_program = glCreateProgram();
	m_program = loadVertexAndFrag(vertex_path.c_str(), fragment_path.c_str()); // Load shaders
	bindAttributes();
	linkProgram();
	m_id = 0;
	
	
}

Shader::Shader(string vertex_path, string fragment_path, string geo_path)
{
	m_program = glCreateProgram();
	m_program = loadVertGeoFrag(vertex_path.c_str(), geo_path.c_str(), fragment_path.c_str());

	bindAttributes();
	linkProgram();
	m_id = 0;

}

Shader::Shader(string vertex_path, string fragment_path, string geo_path, string tessControl_path, string tessEval_path)
{
	m_program = glCreateProgram();

	m_program = loadVertGeoTessFrag(
		vertex_path.c_str(), 
		fragment_path.c_str(), 
		geo_path.c_str(), 
		tessControl_path.c_str(), 
		tessEval_path.c_str());

	bindAttributes();
	linkProgram();
	m_id = 0;
}


Shader::~Shader(){
    
}



string Shader::readFile(const char *filePath){
    string content;

    std::ifstream fileStream(filePath, std::ios::in);
    
    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }
    
    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    
    fileStream.close();
    return content;
}

GLuint Shader::loadVertexAndFrag(const char *vertex_path, const char *fragment_path){
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read shaders
    string vertShaderStr = readFile(vertex_path);
    string fragShaderStr = readFile(fragment_path);
    
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();
    
    
    // Compile vertex shader
    std::cout << "Compiling vertex shader." << std::endl;
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);
    checkShader(vertShader);

    // Compile fragment shader
    std::cout << "Compiling fragment shader." << std::endl;
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);
    checkShader(fragShader);
    
    // Link Shaders
    std::cout << "Linking program" << std::endl;
   
    glAttachShader(m_program, vertShader);
    glAttachShader(m_program, fragShader);
    glLinkProgram(m_program);
    
    // Check program
    GLint result = GL_FALSE;
    int logLength;
    glGetProgramiv(m_program, GL_LINK_STATUS, &result);
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError( (logLength > 1) ? logLength : 1 );
    glGetProgramInfoLog(m_program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;
    
    glDeleteShader(vertShader); // Safe to delete as they are now in the program
    glDeleteShader(fragShader);
    
    return m_program;
}

GLuint Shader::loadVertGeoFrag(const char * vertex_path, const char * geo_path, const char * fragment_path)
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint geoShader = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders
	string vertShaderStr = readFile(vertex_path);
	string geoShaderStr = readFile(geo_path);
	string fragShaderStr = readFile(fragment_path);

	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *geoShaderSrc = geoShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();


	// Compile vertex shader
	std::cout << "Compiling vertex shader." << std::endl;
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);
	checkShader(vertShader);

	// Compile geometry shader
	std::cout << "Compiling geometry shader." << std::endl;
	glShaderSource(geoShader, 1, &geoShaderSrc, NULL);
	glCompileShader(geoShader);
	checkShader(geoShader);

	// Compile fragment shader
	std::cout << "Compiling fragment shader." << std::endl;
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);
	checkShader(fragShader);

	

	// Link Shaders
	std::cout << "Linking program" << std::endl;
	glAttachShader(m_program, vertShader);
	glAttachShader(m_program, fragShader);
	glAttachShader(m_program, geoShader);
	glLinkProgram(m_program);

	// Check program
	GLint result = GL_FALSE;
	int logLength;
	glGetProgramiv(m_program, GL_LINK_STATUS, &result);
	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(m_program, logLength, NULL, &programError[0]);
	std::cout << &programError[0] << std::endl;

	glDeleteShader(vertShader); // Safe to delete as they are now in the program
	glDeleteShader(geoShader);
	glDeleteShader(fragShader);

	return m_program;

}

GLuint Shader::loadVertGeoTessFrag(const char * vertex_path, const char * fragment_path, const char * geo_path, const char * tess_ctr, const char * tess_eval )
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint geoShader = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint tessCrlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
	GLuint tessEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);

	// Read shaders
	string vertShaderStr = readFile(vertex_path);
	string fragShaderStr = readFile(fragment_path);
	string geoShaderStr = readFile(geo_path);
	string tessCtlShaderStr = readFile(tess_ctr);
	string tessEvaShaderStr = readFile(tess_eval);

	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *geoShaderSrc = geoShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();
	const char *tessCtrlShaderSrc = tessCtlShaderStr.c_str();
	const char *tessEvalShaderSrc = tessEvaShaderStr.c_str();


	// Compile vertex shader
	std::cout << "Compiling vertex shader." << std::endl;
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);
	checkShader(vertShader);

	// Compile geometry shader
	std::cout << "Compiling geometry shader." << std::endl;
	glShaderSource(geoShader, 1, &geoShaderSrc, NULL);
	glCompileShader(geoShader);
	checkShader(geoShader);

	// Compile fragment shader
	std::cout << "Compiling fragment shader." << std::endl;
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);
	checkShader(fragShader);

	std::cout << "Compiling Tess control shader." << std::endl;
	glShaderSource(tessCrlShader, 1, &tessCtrlShaderSrc, NULL);
	glCompileShader(tessCrlShader);
	checkShader(tessCrlShader);

	std::cout << "Compiling Tess Evaluation shader." << std::endl;
	glShaderSource(tessEvalShader, 1, &tessEvalShaderSrc, NULL);
	glCompileShader(tessEvalShader);
	checkShader(tessEvalShader);



	// Link Shaders
	std::cout << "Linking program" << std::endl;
	glAttachShader(m_program, vertShader);
	glAttachShader(m_program, fragShader);
	glAttachShader(m_program, geoShader);
	glAttachShader(m_program, tessCrlShader);
	glAttachShader(m_program, tessEvalShader);
	glLinkProgram(m_program);

	// Check program
	GLint result = GL_FALSE;
	int logLength;
	glGetProgramiv(m_program, GL_LINK_STATUS, &result);
	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(m_program, logLength, NULL, &programError[0]);
	std::cout << &programError[0] << std::endl;

	glDeleteShader(vertShader); // Safe to delete as they are now in the program
	glDeleteShader(geoShader);
	glDeleteShader(fragShader);
	glDeleteShader(tessCrlShader);
	glDeleteShader(tessEvalShader);

	return m_program;

	return GLuint();
}

void Shader::checkShader(GLuint shader){
    // Add 'if' check here?
    GLint result = GL_FALSE;
    int logLength;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
    std::cout << &vertShaderError[0] << std::endl;
}

bool Shader::linkProgram(){
	glLinkProgram(m_program);
	
	GLint status;
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	
	if (status == GL_FALSE)	{
		std::cout << "Linking failed! Error log as follows:" << std::endl;
		char error[2048];
		glGetProgramInfoLog(m_program, sizeof(error), NULL, error);
		std::cout << error << std::endl;
		m_linkSuccess = false;

		return false;
	}
	m_linkSuccess = true;
	return true;
}

void Shader::bindAttributes(){
	glBindAttribLocation(m_program, VERTEX_BUFFER,  "position");
	glBindAttribLocation(m_program, COLOUR_BUFFER,  "colour");
	glBindAttribLocation(m_program, NORMAL_BUFFER,  "normal");
	glBindAttribLocation(m_program, TANGENT_BUFFER, "tangent");
	glBindAttribLocation(m_program, TEXTURE_BUFFER, "texCoord");
	
}



