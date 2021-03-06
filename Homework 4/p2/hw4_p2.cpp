// Assignment 4 problem 2

// Author - Payas Awadhutkar

// Based on code given by Dr. Radkowski

// includes
#include <iostream>
#include <string>

#include <GL/glew.h>

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <GLFW/glfw3.h>

// code from instructor Dr. Radkowski
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "Plane3D.h"
#include "CustomTexture.h"

using namespace std;

// standard initiliazations

GLFWwindow* window;
GLuint program;

extern Trackball trackball;
extern int g_change_texture_blend;

GLAppearance* getAppearance() {
    // create an apperance object.
    GLAppearance* appearance = new GLAppearance("multi_texture.vs", "multi_texture.fs");

    // The GLAppearance takes the address of the light source, so we can't use
    // stack variables to create light sources
    GLDirectLightSource* light_source_ptr = new GLDirectLightSource();
    GLDirectLightSource& light_source = *light_source_ptr;
    light_source._lightPos = glm::vec4(0.0, 20.0, 20.0, 0.0);
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 4.5;
    light_source._diffuse_intensity = 1.0;
    light_source._attenuation_coeff = 0.0;

    // add the light to this apperance object
    appearance->addLightSource(light_source);

    GLSpotLightSource* spotlight_source_ptr = new GLSpotLightSource();
    GLSpotLightSource& spotlight_source = *spotlight_source_ptr;
    spotlight_source._lightPos = glm::vec4(0.0, 0.0, 50.0, 1.0);
    spotlight_source._ambient_intensity = 0.2;
    spotlight_source._specular_intensity = 30.5;
    spotlight_source._diffuse_intensity = 8.0;
    spotlight_source._attenuation_coeff = 0.0002;
    spotlight_source._cone_direction = glm::vec3(-1.0, -1.0, -1.0);
    spotlight_source._cone_angle = 20.0;

    appearance->addLightSource(spotlight_source);

    // Create a material object
    GLMaterial* material_ptr = new GLMaterial();
    GLMaterial& material = *material_ptr;
    material._diffuse_material = glm::vec3(0.8, 0.8, 0.0);
    material._ambient_material = glm::vec3(0.8, 0.8, 0.0);
    material._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material._shininess = 12.0;
    material._transparency = 1.0;

    // Add the material to the apperance object
    appearance->setMaterial(material);

    return appearance;
}

int main(int argc, const char * argv[]) {
	 // initiliazations
   
	 window = initWindow();

    initGlew();

    CoordSystem cs(40.0);

    GLAppearance* firstAppearance( getAppearance() );

    // Add a texture for the background display
    CustomTexture* backgroundTexture = new CustomTexture();
    backgroundTexture->loadAndCreateTextures("image1.bmp", "image2.bmp", "image3.bmp");
    backgroundTexture->addVariablesToProgram(firstAppearance->getProgram(), -1);

    // Finalize the appearance object
    firstAppearance->finalize();

    // create the background plane
    GLPlane3D backgroundPlane(0.0, 0.0, 0.0, 50.0, 50.0);
    backgroundPlane.setApperance(*firstAppearance);
    backgroundPlane.init();

    // Code to update appearance
    firstAppearance->updateLightSources();

    // Set up our green background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector.
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 65.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window)) {
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);

        SetTrackballLocation(trackball.getRotationMatrix());

        // draw the objects
        cs.draw();
        backgroundPlane.draw();

        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
