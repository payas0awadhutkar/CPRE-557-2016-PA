/*
 * mysphere.cpp
 *
 * Creates a sphere.
 * Based on GLSphere.cpp provided by Dr. Radkowski
 *
 *  Created on: 05-Oct-2016
 *      Author: Payas Awadhutkar
 */

#include "mysphere.h"

mysphere::mysphere(
		float xcenter,
		float ycenter,
		float zcenter,
		float radius,
		const std::vector<GLSpotLightSource>& spotlight,
		const std::string& vshader,
		const std::string& fshader,
		// diffuse colour
		const glm::vec3& dcolour,
		// ambient colour
		const glm::vec3& acolour,
		// specular colour
		const glm::vec3& scolour,
		int rows,
		int segments)
	:GLSphere(xcenter, ycenter, zcenter, radius, rows, segments),
	 _vshader(vshader),
	 _fshader(fshader),
	 _dcolour(dcolour),
	 _acolour(acolour),
	 _scolour(scolour),
	 _nlocations(0),
	 _light_sources()
{
	_center.x() = xcenter;
	_center.y() = ycenter;
	_center.z() = zcenter;

	_radius = radius;
	_rows = rows;
	_segments = segments;
	_render_normal_vectors = false;
	_program_normals = -1;
	_program = -1;

	for (int i = 0; i < std::min(static_cast<size_t>(8UL), spotlight.size()); ++i) {
	        _light_sources.push_back(spotlight[i]);
	}

	initShader();
}

mysphere::~mysphere() {
	// destructor for cleanup
	glDeleteVertexArrays(1, _vaoID);
	glDeleteVertexArrays(1, _vaoIDNormals);
	glDeleteProgram(_program);
	glDeleteProgram(_program_normals);
}

// Overriding Method for initShader() in GLSphere
// Matrix calculation is the same as GLSphere
// Light sources were added here.
void mysphere::initShader() {
	_program = LoadAndCreateShaderProgram(_vshader, _fshader);

	    glUseProgram(_program);

	    // Vertices

	    glBindAttribLocation(_program, 0, "in_Position");
	    glBindAttribLocation(_program, 1, "in_Normal");
	    glBindAttribLocation(_program, 2, "in_Color");

	    // Define the model view matrix.
	    _modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model

	    _projectionMatrixLocation = glGetUniformLocation(_program, "projectionMatrixBox"); // Get the location of our projection matrix in the shader
	    _viewMatrixLocation = glGetUniformLocation(_program, "viewMatrixBox"); // Get the location of our view matrix in the shader
	    _modelMatrixLocation = glGetUniformLocation(_program, "modelMatrixBox"); // Get the location of our model matrix in the shader
	    _inverseViewMatrixLocation = glGetUniformLocation(_program, "inverseViewMatrix");
	    _nlocations = glGetUniformLocation(_program, "numLights"); // Get the location of the number of lights

	    glUniformMatrix4fv(_projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix()[0][0] ); // Send our projection matrix to the shader
	    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &viewMatrix()[0][0]); // Send our view matrix to the shader
	    glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); // Send our model matrix to the shader
	    glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
	    glUniform1i(_nlocations, (GLint)_light_sources.size()); // Send the number of lights

	    // Material
	    _material._diffuse_material = _dcolour;
	    _material._ambient_material = _acolour;
	    _material._specular_material = _scolour;
	    _material._shininess = 8.0;

	    _material._ambientColorPos = glGetUniformLocation(_program, "ambient_color");
	    _material._diffuseColorPos = glGetUniformLocation(_program, "diffuse_color");
	    _material._specularColorPos = glGetUniformLocation(_program, "specular_color");
	    _material._shininessIdx = glGetUniformLocation(_program, "shininess");

	    // Send the material to your shader program
	    glUniform3fv(_material._ambientColorPos, 1, &_material._ambient_material[0] );
	    glUniform3fv(_material._diffuseColorPos, 1, &_material._diffuse_material[0]);
	    glUniform3fv(_material._specularColorPos, 1, &_material._specular_material[0]);
	    glUniform1f(_material._shininessIdx, _material._shininess);

	    // Light
	    // Add all the light sources
	    for (int i = 0; i < _light_sources.size(); ++i) {
	        GLSpotLightSource& light = _light_sources[i];

	        std::stringstream lightStr;
	        lightStr << "lights[" << i << "]";

	        // Read all the index values from the shader program
	        light._ambientIdx = glGetUniformLocation(_program, std::string(lightStr.str() + ".ambient_intensity").c_str());
	        light._diffuseIdx = glGetUniformLocation(_program, std::string(lightStr.str() + ".diffuse_intensity").c_str());
	        light._specularIdx = glGetUniformLocation(_program, std::string(lightStr.str() + ".specular_intensity").c_str());
	        light._attenuation_coeffIdx = glGetUniformLocation(_program, std::string(lightStr.str() + ".attenuationCoefficient").c_str());
	        light._lightPosIdx = glGetUniformLocation(_program, std::string(lightStr.str() + ".pos").c_str());
	        light._cone_angleIdx = glGetUniformLocation(_program, std::string(lightStr.str() + ".cone_angle").c_str());
	        light._cone_directionIdx = glGetUniformLocation(_program, std::string(lightStr.str() + ".cone_direction").c_str());

	        // Send the light information to your shader program
	        glUniform1f(light._ambientIdx, light._ambient_intensity);
	        glUniform1f(light._diffuseIdx, light._diffuse_intensity);
	        glUniform1f(light._specularIdx, light._specular_intensity);
	        glUniform1f(light._attenuation_coeffIdx, light._attenuation_coeff);
	        glUniform4fv(light._lightPosIdx, 1, &light._lightPos[0]);

	        glUniform1f(light._cone_angleIdx, light._cone_angle);
	        glUniform3fv(light._cone_directionIdx, 1, &light._cone_direction[0]);
	    }

	    glUseProgram(0);
}

