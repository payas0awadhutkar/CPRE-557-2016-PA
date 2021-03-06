/*
 * hw3.cpp
 *
 *  Created on: 07-Oct-2016
 *      Author: Payas Awadhutkar
 */

#include <iostream>
#include <string>

#include <GL/glew.h>

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <GLFW/glfw3.h>

// these were provided by course instructor, Dr. Radkowski
// taken from gl_common
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "mysphere.h"

using namespace std;

GLFWwindow* window;
GLuint program;

// Trackball to rotate and move camera
extern Trackball trackball;

/*
 * Next four functions create a sphere each
 * All cone angles are in degrees
 * Every sphere has its own light source
 */
mysphere build_sphere_red() {
    std::vector<GLSpotLightSource> lights;
    GLSpotLightSource light_source;

    // Red sphere
    light_source._lightPos = glm::vec4(25.0, 25.0, 0.0, 0.0);
    light_source._ambient_intensity = 0.0;
    light_source._specular_intensity = 0.0;
    light_source._diffuse_intensity = 10.0;
    light_source._attenuation_coeff = 0.02;

    light_source._cone_angle = 0.0;
    light_source._cone_direction = glm::vec3(-1.0, -1.0, 0.0);
    lights.push_back(light_source);

    // White spotlight
    light_source._lightPos = glm::vec4(15.0, 15.0, 0.0, 0.0);
    light_source._ambient_intensity = 0.0;
    light_source._specular_intensity = 5.0;
    light_source._diffuse_intensity = 0.0;
    light_source._attenuation_coeff = 0.02;

    light_source._cone_angle = 50.0;
    light_source._cone_direction = glm::vec3(-1.0, -1.0, 0.0);
    lights.push_back(light_source);

    return mysphere(
        -20.0,                      // xcenter
        0.0,                        // ycenter
        0.0,                        // zcenter
        5.0,                        // radius
        lights,                     // lights
        "vshader.vs",      // the vertex shader
        "fshader.fs",      // the fragment shader
        glm::vec3(1.0, 0.0, 0.0),   // diffuse color
        glm::vec3(1.0, 0.0, 0.0),   // ambient color
        glm::vec3(1.0, 1.0, 1.0),   // specular color
        50,                         // rows
        50                          // segments
    );
}

mysphere build_sphere_blue() {
    std::vector<GLSpotLightSource> lights;
    GLSpotLightSource light_source;

    // Blue sphere
    light_source._lightPos = glm::vec4(25.0, 25.0, 0.0, 0.0);
    light_source._ambient_intensity = 0.0;
    light_source._specular_intensity = 0.0;
    light_source._diffuse_intensity = 10.0;
    light_source._attenuation_coeff = 0.02;

    light_source._cone_angle = 0.0;
    light_source._cone_direction = glm::vec3(-1.0, -1.0, 0.0);
    lights.push_back(light_source);

    return mysphere(
        -7.0,                       // xcenter
        0.0,                        // ycenter
        0.0,                        // zcenter
        5.0,                        // radius
        lights,                     // lights
        "vshader.vs",      // the vertex shader
        "fshader.fs",      // the fragment shader
        glm::vec3(0.0, 0.0, 1.0),   // diffuse color
        glm::vec3(1.0, 1.0, 1.0),   // ambient color
        glm::vec3(1.0, 1.0, 1.0),   // specular color
        50,                         // rows
        50                          // segments
    );
}

mysphere build_sphere_green() {
    std::vector<GLSpotLightSource> lights;
    GLSpotLightSource light_source;

    // Green sphere
    light_source._lightPos = glm::vec4(10.0, 4.0, 7.0, 0.0);
    light_source._ambient_intensity = 0.0;
    light_source._specular_intensity = 1.0;
    light_source._diffuse_intensity = 10.0;
    light_source._attenuation_coeff = 0.02;

    light_source._cone_angle = 90.0;
    light_source._cone_direction = glm::vec3(-1.0, -1.0, -1.0);
    lights.push_back(light_source);

    return mysphere(
        7.0,                        // xcenter
        0.0,                        // ycenter
        0.0,                        // zcenter
        5.0,                        // radius
        lights,                     // light
        "vshader.vs",      // the vertex shader
        "fshader.fs",      // the fragment shader
        glm::vec3(0.0, 1.0, 0.0),   // diffuse color
        glm::vec3(1.0, 1.0, 1.0),   // ambient color
        glm::vec3(1.0, 1.0, 1.0),   // specular color
        50,                         // rows
        50                          // segments
    );
}

mysphere build_sphere_golden() {
    std::vector< GLSpotLightSource > lights;
    GLSpotLightSource light_source;

    // Golden sphere
    light_source._lightPos = glm::vec4(25.0, 15.0, 4.0, 0.0);
    light_source._ambient_intensity = 0.3;
    light_source._specular_intensity = 0.0;
    light_source._diffuse_intensity = 2.0;
    light_source._attenuation_coeff = 0.02;

    light_source._cone_angle = 90.0;
    light_source._cone_direction = glm::vec3(-1.0, -1.0, -1.0);
    lights.push_back(light_source);

	 // Spotlight
    light_source._lightPos = glm::vec4(25.0, 15.0, 6.0, 0.0);
    light_source._ambient_intensity = 0.0;
    light_source._specular_intensity = 2.0;
    light_source._diffuse_intensity = 0.0;
    light_source._attenuation_coeff = 0.02;

    light_source._cone_angle = 95.0;
    light_source._cone_direction = glm::vec3(-1.0, -1.0, -1.0);
    lights.push_back(light_source);

    return mysphere(
        20.0,                       // xcenter
        0.0,                        // ycenter
        0.0,                        // zcenter
        5.0,                        // radius
        lights,                     // lights
        "vshader.vs",      // the vertex shader
        "fshader.fs",      // the fragment shader
        glm::vec3(0.83, 0.67, 0.22),// diffuse color
        glm::vec3(0.83, 0.67, 0.22),// ambient color
        glm::vec3(1.0, 1.0, 1.0),   // specular color
        50,                         // rows
        50                          // segments
    );
}

int main(int argc, const char* argv[]) {

    window = initWindow();
    initGlew();

    // coordinate system taken from Coordsystem.h
    CoordSystem xy(40.0);

	 // 4 spheres
    mysphere sphere_1(build_sphere_red());
    mysphere sphere_2(build_sphere_blue());
    mysphere sphere_3(build_sphere_green());
    mysphere sphere_4(build_sphere_golden());

	 // Setting up background, camera, depth test
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    SetViewAsLookAt(glm::vec3(30.0f, 30.0f, 30.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glEnable(GL_DEPTH_TEST);

    // Infinite loop to keep window open
    while (!glfwWindowShouldClose(window))
    {
        // Clear buffer
        glClearBufferfv(GL_COLOR, 0, clear_color);
        glClearBufferfv(GL_DEPTH, 0, clear_depth);

        // Trackball
        SetTrackballLocation(trackball.getRotationMatrix());

        // draw the objects
        xy.draw();
        sphere_1.draw();
        sphere_2.draw();
        sphere_3.draw();
        sphere_4.draw();

        // Display the objects
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
