//
//  main.cpp
//  OpenGL4Test
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// Assignment 2 - 3D modelling
// TODOs/missing code completed by Payas Awadhutkar
// Also removed some extra whitespaces

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"

// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;

static const string vs_string =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrix;                                    \n"
"uniform mat4 viewMatrix;                                           \n"
"uniform mat4 modelMatrix;                                          \n"
"in vec3 in_Position;                                               \n"
"                                                                   \n"
"in vec3 in_Color;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
"    pass_Color = in_Color;                                         \n"
"}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string =
"#version 410 core                                                 \n"
"                                                                  \n"
"in vec3 pass_Color;                                                 \n"
"out vec4 color;                                                    \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";


/// Camera control matrices
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix


// The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Fill this functions with your model code.

// USE THESE vertex array objects to define your objects
unsigned int vaoID[2];

unsigned int vboID[4];

/*!
 ADD YOUR CODE TO CREATE THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 * code added - payas
 */

// Declaring variables and allocating memory before creating any model. 
 
// objects to store triangle strip model vertices and their colours
int triangle_strip_vertex_count = 500;
float* triangle_strip_vertices;
float* triangle_strip_colours;

// objects to store triangle model vertices and their colours
int triangle_vertex_count = 500;
float* triangle_vertices;
float* triangle_colours;

// Adding code to create triangle strip model here onwards.

unsigned int createTriangleStripModel(void)
{
    // use the vertex array object vaoID[0] for this model representation
	
	// Some portions of some faces are included in multiple triangles,
    // due to the ordering of the triangle vertices, and the use of a
	// single triangle strip
	
	// Given 3D model can be viewed as three solids glued together.
	// A horizontal rectangular prism, a vertical/erect rectangular prism and a trapezoid
	// I define vertices w.r.t. this convention to make the understanding easier
	// I follow same convention with triangle model.
	
	triangle_strip_vertices = new float[triangle_strip_vertex_count] {
	// Left side square face of horizontal rectangular prism
		0, 0, 3, 
		1, 0, 3,
		0, 1, 3,
		1, 1, 3,
	// Top rectangular face of the horizontal rectangular prism
		0, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 3,
	// Front rectangular face of the horizontal rectangular prism
		1, 0, 1,
		1, 0, 3,
	// Bottom rectangular face of the horizontal rectangular prism
		0, 0, 1, // 7
		0, 0, 3, // 8
	// Back side face of the horizontal rectangular prism
		0, 1, 1,
		0, 1, 3,
		0, 0, 1,
	// Back side face of the vertical/erect rectangular prism
		0, 0, 0,
		0, 2, 1,
		0, 2, 0,
	// Right side face of the vertical/erect rectangular prism
		1, 2, 0,
		0, 0, 0,
		1, 0, 0,
	// Left side face of the vertical/erect rectangular prism
	// Note : Only a portion of this face is visible as horizontal prism covers it.
		1, 1, 1,
		0, 1, 1,
		1, 2, 1,
		0, 2, 1,
	// Completing top face of the vertical/erect rectangular prism
		1, 2, 0,
		1, 2, 1,
	// Completing Slanting rectangular face of the trapezoid
		3, 1, 0,
		3, 1, 1,
	// Completing Front square face of the trapezoid
		3, 0, 0,
		3, 0, 1,
	// Completing Bottom face of the trapezoid and vertical/erect prism
		0, 0, 0,
		0, 0, 1,
	// Completing Bottom face of the horizontal rectangular prism
		1, 0, 1,
		3, 0, 1,
	// Completing left side trapezium face of the trapezoid
		1, 1, 1,
		3, 1, 1,
		1, 2, 1,
	// Right side trapezium face of the trapezoid
		1, 0, 0,
		3, 0, 0,
		1, 1, 0,
		3, 1, 0,
		1, 2, 0,
	};

	triangle_strip_colours = new float[triangle_strip_vertex_count];

	// Assigning random float values to colours
	// Picked this idea from here - http://www.dreamincode.net/forums/topic/177692-colours-in-opengl/
	for (unsigned int i = 0; i < triangle_strip_vertex_count; i++) {
		triangle_strip_colours[i] = static_cast <float> (rand())/static_cast <float> (RAND_MAX);
	}
	return 1;
}

/*!
 ADD YOUR CODE TO CREATE A MODEL USING PRIMITIVES OF YOUR CHOISE TO THIS FUNCTION
 * code added - payas
 */
 
 
// Polygons didn't work. Thus, I used triangles to create this model. 
unsigned int createPolygonModel(void)
{
	// This is actually createTriangleModel - as in that's what it does
	// Coordinates of the vertices remain same as triangle strip model
	// Difference lies in arranging them in the array
		
	triangle_vertices = new float[triangle_vertex_count] {
	// Following two triangles complete left side square face of the horizontal rectangular prism 
		0, 0, 3,
		1, 0, 3,
		0, 1, 3,
		1, 0, 3,
		0, 1, 3,
		1, 1, 3,
	// Following two triangles complete top rectangular face of the horizontal rectangular prism
		0, 1, 3,
		1, 1, 3,
		0, 1, 1,
		1, 1, 3,
		0, 1, 1,
		1, 1, 1,
	// Following two triangles complete front face of the horizontal rectangular prism
		1, 1, 1,
		1, 1, 3,
		1, 0, 1,
		1, 1, 3,
		1, 0, 1,
		1, 0, 3,
	// Following two triangles complete bottom face of the horizontal rectangular prism
		1, 0, 1,
		1, 0, 3,
		0, 0, 3,
		1, 0, 1,
		0, 0, 3,
		0, 0, 1,
	// Following two triangles complete back side face of the horizontal rectangular prism
		0, 0, 1,
		0, 0, 3,
		0, 1, 1,
		0, 0, 3,
		0, 1, 1,
		0, 1, 3,
	// Following two triangles complete back side face of the horizontal rectangular prism 
		0, 0, 1,
		0, 0, 0,
		0, 2, 1,
		0, 0, 0,
		0, 2, 1,
		0, 2, 0,
	// Portion of top face of vertical/erect rectangular prism
		0, 2, 1,
		0, 2, 0,
		1, 2, 0,
	// Following two triangles complete right side face vertical/erect rectangular prism
		0, 2, 0,
		1, 2, 0,
		0, 0, 0,
		1, 2, 0,
		0, 0, 0,
		1, 0, 0,
	// Following two triangles complete visible part of left face of vertical/erect rectangular prism
		1, 1, 1,
		0, 1, 1,
		1, 2, 1,
		0, 1, 1,
		1, 2, 1,
		0, 2, 1,
	// Remaining portion of top face of vertical/erect rectangular prism
	// I tried to group it with its first portion but rendering result was weird
		1, 2, 1,
		0, 2, 1,
		1, 2, 0,
	// Connecting top face to complete vertical/erect prism
		0, 2, 1,
		1, 2, 0,
		1, 2, 1,
	// Following two triangles complete slanting face of the trapezoid
		1, 2, 0,
		1, 2, 1,
		3, 1, 0,
		1, 2, 1,
		3, 1, 0,
		3, 1, 1,
	// Following two triangles complete front face of the trapezoid
		3, 1, 0,
		3, 1, 1,
		3, 0, 0,
		3, 1, 1,
		3, 0, 0,
		3, 0, 1,
	// Following two triangles complete bottom face the trapezoid and vertical/erect prism.
		3, 0, 0,
		3, 0, 1,
		0, 0, 0,
		3, 0, 1,
		0, 0, 0,
		0, 0, 1,
	// Following two triangles complete rectangular portion of left face of trapezoid
		1, 0, 1,
		3, 0, 1,
		1, 1, 1,
		3, 0, 1,
		1, 1, 1,
		3, 1, 1,
	// Following triangle completes triangular portion of left face of trapezoid
		1, 1, 1,
		3, 1, 1,
		1, 2, 1,
	// Following two triangles complete rectangular portion of right face of trapezoid
		1, 0, 0,
		3, 0, 0,
		1, 1, 0,
		3, 0, 0,
		1, 1, 0,
		3, 1, 0,
	// Following triangle completes triangular portion of right side face of the trapezoid
		1, 1, 0,
		3, 1, 0,
		1, 2, 0,
	};

	triangle_colours = new float[triangle_vertex_count];

	// Similar to triangle strips assigning random colours
	for (unsigned int i = 0; i < triangle_vertex_count; i++) {
		triangle_colours[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}

	return 1;
}


/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 * code added - payas
 */
void renderTriangleStripModel(void)
{

	// Bind the buffer and switch it to an active buffer
	glGenVertexArrays(2, &vaoID[0]);
	glBindVertexArray(vaoID[0]);

	glGenBuffers(2, vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, triangle_strip_vertex_count * sizeof(GLfloat), triangle_strip_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, triangle_strip_vertex_count * sizeof(GLfloat), triangle_strip_colours, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY TRIANGLES / VERTICES YOU HAVE.
	// COMPLETE THE LINE
	// Draw the triangles
	
	//Code to draw triangle strips
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	glDrawArrays(GL_TRIANGLE_STRIP, 5, 9);
	glDrawArrays(GL_TRIANGLE_STRIP, 14, 7);
	glDrawArrays(GL_TRIANGLE_STRIP, 21, 12);
	glDrawArrays(GL_TRIANGLE_STRIP, 33, 5);
	glDrawArrays(GL_TRIANGLE_STRIP, 38, 5);

	// Unbind our Vertex Array Object
	glBindVertexArray(0);
}


/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 * code added - payas
 */
void renderPolygonModel(void)
{

	// Bind the buffer and switch it to an active buffer
	glGenVertexArrays(2, &vaoID[0]);
	glBindVertexArray(vaoID[0]);

	glGenBuffers(2, vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, triangle_vertex_count * sizeof(GLfloat), triangle_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, triangle_vertex_count * sizeof(GLfloat), triangle_colours, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY TRIANGLES / VERTICES YOU HAVE.
	// COMPLETE THE LINE
	// Draw the triangles
	
	// Code to draw triangles
	// It is possible to call glDrawArrays() in loop for triangles
	// Because for every triangle, all 3 vertices are listed explicitly. So count can be fixed to 3
	// unlike triangle strips which is combination of multiple triangles and thus a fixed pattern is missing
	// Also it is convenient to write code this way and it also makes it more readable
	for (unsigned int i = 0; i < 31; i++) {
		glDrawArrays(GL_TRIANGLES, i * 3, 3);
	}

	// Unbind our Vertex Array Object
	glBindVertexArray(0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 This function creates the two models
 */
void setupScene(void) {

	// triangle strip model
	createTriangleStripModel();
	renderTriangleStripModel();

	// triangle model - in place of polygon model
	createPolygonModel();
	renderPolygonModel();
}

int main(int argc, const char * argv[])
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Init glfw, create a window, and init glew

	// Init the GLFW Window
	window = initWindow();


	// Init the glew api
	initGlew();

	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// The Shader Program starts here

	// Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
	// Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
	static const string vertex_code = vs_string;
	static const char * vs_source = vertex_code.c_str();

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	static const string fragment_code = fs_string;
	static const char * fs_source = fragment_code.c_str();

	// This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
	program = glCreateProgram();

	// We create a shader with our fragment shader source code and compile it.
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);

	// We create a shader with our vertex shader source code and compile it.
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);

	// We'll attach our two compiled shaders to the OpenGL program.
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	// We'll specify that we want to use this program that we've attached the shaders to.
	glUseProgram(program);

	//// The Shader Program ends here
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	/// IGNORE THE NEXT PART OF THIS CODE
	/// IGNORE THE NEXT PART OF THIS CODE
	/// IGNORE THE NEXT PART OF THIS CODE
	// It controls the virtual camera

	// Set up our green background color
	static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
	static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };


	projectionMatrix = glm::perspective(1.1f, (float)800 / (float)600, 0.1f, 100.f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
	viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
	int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
	int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader


	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader


	 //// The Shader Program ends here
	//// START TO READ AGAIN
	//// START TO READ AGAIN
	//// START TO READ AGAIN
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glBindAttribLocation(program, 0, "in_Position");
	glBindAttribLocation(program, 1, "in_Color");

	// this creates the scene
	setupScene();

	int i = 0;

	// Enable depth test
	// ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
	glEnable(GL_DEPTH_TEST);

	// This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
	while (!glfwWindowShouldClose(window))
	{

		// Clear the entire buffer with our green color (sets the background to be green).
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);

		// this draws the coordinate system
		coordinate_system_renderer->draw();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// This generate the object
		// Enable the shader program
		glUseProgram(program);

		// this changes the camera location
		glm::mat4 rotated_view = viewMatrix * GetRotationMatrix();
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader


		// This moves the model to the right
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader


		// This line renders your triangle strip model
		renderTriangleStripModel();


		// This moves the model to the left
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader

		// This line renders your Ppolygon model
		renderPolygonModel();


		// disable the shader program
		glUseProgram(0);


		//// This generate the object
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// Swap the buffers so that what we drew will appear on the screen.
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// delete the coordinate system object
	delete coordinate_system_renderer;

	// Program clean up when the window gets closed.
	glDeleteVertexArrays(2, vaoID);
	glDeleteProgram(program);
}
