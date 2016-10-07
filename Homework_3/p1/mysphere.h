/*
 * mysphere.h
 *  header file for mysphere.cpp
 *  Created on: 05-Oct-2016
 *      Author: Payas Awadhutkar
 */

#ifndef MYSPHERE_H_
#define MYSPHERE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLSphere.h"
#include "GLObject.h"
#include "Shaders.h"
#include "HCI557Datatypes.h"

class mysphere : public GLSphere {
public :
	mysphere(
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
			int rows = 10,
			int segments = 10
	);
	~mysphere();

protected :
	// init shader
	virtual void initShader();

	std::string _vshader;
	std::string _fshader;
	glm::vec3 _dcolour;
	glm::vec3 _acolour;
	glm::vec3 _scolour;
	int _nlocations;
	std::vector<GLSpotLightSource> _light_sources;
};

#endif /* MYSPHERE_H_ */

