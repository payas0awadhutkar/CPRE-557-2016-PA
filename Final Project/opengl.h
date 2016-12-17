
#ifndef OPENGL_H
#define OPENGL_H



#include <windows.h>		// Header File For Windows
#include <stdlib.h>
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <GL/glut.h>
#include <wingdi.h>

#include <vector>
using namespace std;

#include "xml_model.h"
#include "settings.h"
#include "tga.h"
#include "objects.h"

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

GLuint texture,texture_tree; //the array for our texture
GLTexture skybox_texture[6];

vector<XMLModel> models_xml;

static float angle=0.0,ratio;
//static float x=0.0f,y=1.75f,z=5.0f;
//static float lx=0.0f,ly=0.0f,lz=-1.0f;
static GLint snowman_display_list;

const int MAX_KEYS=256;
bool gKeys[MAX_KEYS];

GLuint LoadTexture( const char * filename, int width, int height )
{
	GLuint texture;
	unsigned char * data;
	FILE * file;

	//The following code will read in our RAW file
	file = fopen( filename, "rb" );
	if ( file == NULL ) return 0;
	data = (unsigned char *)malloc( width * height * 3 );
	fread( data, width * height * 3, 1, file );
	fclose( file );

	glGenTextures( 1, &texture ); //generate the texture with the loaded data
	glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); //set texture environment parameters

	//here we are setting what textures to use and when. The MIN filter is which quality to show
	//when the texture is near the view, and the MAG filter is which quality to show when the texture
	//is far from the view.

	//The qualities are (in order from worst to best)
	//GL_NEAREST
	//GL_LINEAR
	//GL_LINEAR_MIPMAP_NEAREST
	//GL_LINEAR_MIPMAP_LINEAR

	//And if you go and use extensions, you can use Anisotropic filtering textures which are of an
	//even better quality, but this will do for now.
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	//Here we are setting the parameter to repeat the texture instead of clamping the texture
	//to the edge of our shape. 
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR );

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free( data ); //free the texture
	return texture; //return whether it was successfull
}

GLuint LoadTGATexture(char *file)
{
	GLuint t;
	tgaInfo *image;
	image = tgaLoad(file);

	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D,t);

	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,	GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,	GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER	,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER	,GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 
				0, GL_RGBA, GL_UNSIGNED_BYTE, image->imageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return t;
}
AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

int LoadTexture(char* pic,GLuint *x)									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator
    
	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	//memset(TextureImage,0,sizeof(void *)*2);           	// Set The Pointer To NULL
	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP(pic))
	{
		Status=TRUE;					
		glGenTextures(1, x);				
		glBindTexture(GL_TEXTURE_2D,*x);

		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,	GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,	GL_REPEAT);
		
		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER	,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER	,GL_LINEAR);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}

    if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
   			free(TextureImage[0]->data);                            // Free The Texture Image Memory 
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	return Status;										// Return The Status
}

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	//if(!LoadTexture("data/tree.bmp",&texture))return 0;
	//texture =  LoadTexture( "data/nehe.bmp",20,20);
	//texture_tree=LoadTGATexture("data/tree.tga");

	skybox_texture[4].Load("data/images/M5l2_ft.jpg");
	skybox_texture[5].Load("data/images/M5l2_bk.jpg");
	skybox_texture[3].Load("data/images/M5l2_lf.jpg");
	skybox_texture[2].Load("data/images/M5l2_rt.jpg");
	skybox_texture[0].Load("data/images/M5l2_up.jpg");
	skybox_texture[1].Load("data/images/M5l2_ft.jpg");

	//if(!LoadTexture("images/M5l2_bk.bmp",&skybox_texture[5]))return 0;
	//if(!LoadTexture("images/M5l2_lf.bmp",&skybox_texture[3]))return 0;
	//if(!LoadTexture("images/M5l2_rt.bmp",&skybox_texture[2]))return 0;
	//if(!LoadTexture("images/M5l2_up.bmp",&skybox_texture[0]))return 0;
	//if(!LoadTexture("images/sky.bmp",&skybox_texture[1]))return 0;

	return 1;
}

/*
void glPrint(GLuint font, int x, int y, const char text[]){
	float tx, ty;
	float unit = 1.0/16;
	glBindTexture(GL_TEXTURE_2D, font);
	glBegin(GL_QUADS);
	for(int i=0; text[i]!='\0'; i++){
		tx = (int)(text[i]%16  )*unit;
		ty = (int)(text[i]*unit)*unit;

		glTexCoord2f(tx     , ty     ); glVertex2i(x+textSize[0]*i            , y            );
		glTexCoord2f(tx+unit, ty     ); glVertex2i(x+textSize[0]*i+textSize[0], y            );
		glTexCoord2f(tx+unit, ty+unit); glVertex2i(x+textSize[0]*i+textSize[0], y+textSize[1]);
		glTexCoord2f(tx     , ty+unit); glVertex2i(x+textSize[0]*i            , y+textSize[1]);
	}
	glEnd();
}*/
void renderStrokeCharacter(float x, float y, float z, void *font,char *string)
{
  
	  char *c;
	  glPushMatrix();
	  glTranslatef(x, y, z);
	  for (c=string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	  }
	  glPopMatrix();
}


int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
	{
		return FALSE;									// If Texture Didn't Load Return FALSE
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return TRUE;										// Initialization Went OK
}
void changeSize(int w, int h)
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)h = 1;

	ratio = 1.0f * 1366 / 768;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(60,ratio,1,500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*gluLookAt(xListenerPos, yListenerPos, zListenerPos, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);*/
}

void FreeTexture( GLuint texture )
{
  glDeleteTextures( 1, &texture ); 
}

void square (void) {
	glBindTexture( GL_TEXTURE_2D, texture ); //bind our texture to our shape
	glBegin (GL_QUADS);
	glTexCoord2d(0.0,0.0); glVertex2d(-1.0,-1.0); //with our vertices we have to assign a texcoord
    glTexCoord2d(1.0,0.0); glVertex2d(+1.0,-1.0); //so that our texture has some points to draw to
    glTexCoord2d(1.0,1.0); glVertex2d(+1.0,+1.0);
    glTexCoord2d(0.0,1.0); glVertex2d(-1.0,+1.0);
	glEnd();
}

int CalculateFPS()
{
	static int iCurrentTick = 0,  iFps = 0, iFrames = 0;
	static int iStartTick = GetTickCount();

	iFrames++;
    iCurrentTick = GetTickCount();
	if ((iCurrentTick - iStartTick) >= 1000) 
	{
		iFps = (int)((float)iFrames/(iCurrentTick-iStartTick)*1000.0f);
		iFrames = 0;
		iStartTick = iCurrentTick;
	}
	return iFps;
}

void DrawLine(hkVector4 &v1,hkVector4 &v2,hkVector4 &color)
{
	glPushMatrix();
		 glDisable(GL_LIGHTING);
		 glColor3f(color(0),color(1),color(2));
		 glBegin(GL_LINES);
			 glVertex3f(v1(0),v1(1),v1(2));
			 glVertex3f(v2(0),v2(1),v2(2));
		 glEnd();
		 glEnable(GL_LIGHTING);
	glPopMatrix();
}

float lineLength(hkVector4 &v1,hkVector4 &v2)
{
	return ( sqrt( (v1(0)-v2(0))*(v1(0)-v2(0)) + (v1(1)-v2(1))*(v1(1)-v2(1))+ (v1(2)-v2(2))*(v1(2)-v2(2)) ));
}

void display (void) {
	glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT);
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ZERO);
    glLoadIdentity(); 
	glEnable( GL_TEXTURE_2D );
	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//m.Draw();
	glutSwapBuffers();
	//angle ++;
}


//other functions

void l3dBillboardCheatSphericalBegin() {
	
	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) {
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}

	// set the modelview with no rotations
	glLoadMatrixf(modelview);
}

 
inline void l3dBillboardEnd() {

	// restore the previously stored modelview matrix
	glPopMatrix();
}

void DrawTrees() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
	//glDisable(GL_LIGHT0);
   // glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_COLOR_MATERIAL_FACE );

	glBindTexture(GL_TEXTURE_2D,texture_tree);
	for( int i = -5; i < 5; i++) {
		for(int j = -5; j < 5; j++) {
			glPushMatrix();
				glTranslatef(5+i*20.0,0,5+j * 20.0);
				l3dBillboardCheatSphericalBegin();
					glBegin(GL_QUADS);
							glTexCoord2f(0,0);glVertex3f(-3.0f, 0.0f, 0.0f);
							glTexCoord2f(1,0);glVertex3f(3.0f, 0.0f, 0.0f);
							glTexCoord2f(1,1);glVertex3f(3.0f, 6.0f,  0.0f);
							glTexCoord2f(0,1);glVertex3f(-3.0f, 6.0f,  0.0f);
					glEnd();
				l3dBillboardEnd();
			glPopMatrix();
		}
	}

	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
}

void RenderSkybox(hkVector4 &position,hkVector4 &size,GLTexture *SkyBox) {	

	glDisable(GL_DEPTH_TEST);
	// Save Current Matrix
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glDisable(GL_COLOR_MATERIAL_FACE );
 
	// Second Move the render space to the correct position (Translate)
	
	// First apply scale matrix
	//float modelview[16];
	//glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
	//glTranslatef(modelview[3],modelview[7],modelview[11]);

	glTranslatef(position(0),position(1),position(2));


	glScalef(size(0),size(1),size(2));
	glRotatef(-90,1,0,0);
	float cz = -0.0f,cx = 1.0f;
	float r = 1.005f; // If you have border issues change this to 1.005f
	// Common Axis Z - FRONT Side

	SkyBox[4].Use();
	glBegin(GL_QUADS);	
		glTexCoord2f(cx, cz); glVertex3f(-r ,1.0f,-r);
		glTexCoord2f(cx,  cx); glVertex3f(-r,1.0f,r);
		glTexCoord2f(cz,  cx); glVertex3f( r,1.0f,r); 
		glTexCoord2f(cz, cz); glVertex3f( r ,1.0f,-r);
	glEnd();
 
	// Common Axis Z - BACK side
	SkyBox[5].Use();
	glBegin(GL_QUADS);		
		glTexCoord2f(cx,cz);  glVertex3f(-r,-1.0f,-r);
		glTexCoord2f(cx,cx);  glVertex3f(-r,-1.0f, r);
		glTexCoord2f(cz,cx);  glVertex3f( r,-1.0f, r); 
		glTexCoord2f(cz,cz);  glVertex3f( r,-1.0f,-r);
	glEnd();
 
	// Common Axis X - Left side
	SkyBox[3].Use();
	glBegin(GL_QUADS);		
		glTexCoord2f(cx,cx); glVertex3f(-1.0f, -r, r);	
		glTexCoord2f(cz,cx); glVertex3f(-1.0f,  r, r); 
		glTexCoord2f(cz,cz); glVertex3f(-1.0f,  r,-r);
		glTexCoord2f(cx,cz); glVertex3f(-1.0f, -r,-r);		
	glEnd();
 
	// Common Axis X - Right side
	SkyBox[2].Use();
	glBegin(GL_QUADS);		
		glTexCoord2f( cx,cx); glVertex3f(1.0f, -r, r);	
		glTexCoord2f(cz, cx); glVertex3f(1.0f,  r, r); 
		glTexCoord2f(cz, cz); glVertex3f(1.0f,  r,-r);
		glTexCoord2f(cx, cz); glVertex3f(1.0f, -r,-r);
	glEnd();
 
	// Common Axis Y - Draw Up side
	SkyBox[0].Use();
	glBegin(GL_QUADS);		
		glTexCoord2f(cz, cz); glVertex3f( r, -r,1.0f);
		glTexCoord2f(cx, cz); glVertex3f( r,  r,1.0f); 
		glTexCoord2f(cx, cx); glVertex3f(-r,  r,1.0f);
		glTexCoord2f(cz, cx); glVertex3f(-r, -r,1.0f);
	glEnd();
 
	// Common Axis Y - Down side
	SkyBox[1].Use();
	glBegin(GL_QUADS);		
		glTexCoord2f(cz,cz);  glVertex3f( r, -r,-1.0f);
		glTexCoord2f( cx,cz); glVertex3f( r,  r,-1.0f); 
		glTexCoord2f( cx,cx); glVertex3f(-r,  r,-1.0f);
		glTexCoord2f(cz, cx); glVertex3f(-r, -r,-1.0f);
	glEnd();
 

	// Load Saved Matrix
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
 
};

char* ArgsString(char * format, ...)
{
  char *buffer=new char[256];
  va_list args;
  va_start (args, format);
	vsprintf(buffer,format, args);
  va_end (args);

  return buffer;
}


// Here are the fonts:
LPVOID glutFonts[7] = {
    GLUT_BITMAP_9_BY_15,
    GLUT_BITMAP_8_BY_13,
    GLUT_BITMAP_TIMES_ROMAN_10,
    GLUT_BITMAP_TIMES_ROMAN_24,
    GLUT_BITMAP_HELVETICA_10,
    GLUT_BITMAP_HELVETICA_12,
    GLUT_BITMAP_HELVETICA_18
};

// Here is the function
void glutPrint(float x, float y, LPVOID font, char* text, float r, float g, float b, float a,float z=-6.0f)
{
    if(!text || !strlen(text)) return;
    bool blending = false;
    if(glIsEnabled(GL_BLEND)) blending = true;
	glPushMatrix();
		glLoadIdentity();
		glDisable(GL_DEPTH);
		glDisable(GL_LIGHTING);
		//glTranslatef(2.5f,1.2f,-6.0f);
		glScalef(2,2,2);
		glTranslatef(x,y,z);
		glColor4f(r,g,b,a);
		glRasterPos2f(1,1);
		while (*text) {
			glutBitmapCharacter(font, *text);
			text++;
		}
		if(!blending) glDisable(GL_BLEND);
		glEnable(GL_DEPTH);
		glEnable(GL_LIGHTING);
	glPopMatrix();
}  
void glutPrintX(LPVOID font, char* text,float x,float y) {
	glPushMatrix();
		glDisable(GL_DEPTH);
		glRasterPos2f(x,y);
		while (*text) {
			glutBitmapCharacter(font, *text);
			text++;
		}
		glEnable(GL_DEPTH);
	glPopMatrix();
}  

hkReal PI=3.14159265;
hkReal StanderizeAngle(float angle) {
	float a=angle;
	if( a < 0)a=2*PI+a;
	else if( a > 2*PI) a= a-2*PI;

	return a;
}
bool isbetween(float x,float min_angle,float max_angle) {
	bool r_flag=0;
	if( max_angle > 2*PI) {
		r_flag=1;
		max_angle=max_angle-2*PI;
	}
	else if( min_angle < 0 ) {
		r_flag=1;
		min_angle=2*PI-abs(min_angle);
	}
	
	float Big,Small;
	if(min_angle>max_angle) {
		Big=min_angle;
		Small=max_angle;
	}
	else {
		Big=max_angle;
		Small=min_angle;
	}

	if(x>Small && x<Big)	{
		if(r_flag==1)return 0;
		return 1;
	}
	else {
		if(r_flag==1)return 1;
		return 0;
	}
}

Setting s;

int LoadModels() {
	char str[256];
	s.GetSettings("data/settings");

	for(int i=0;i<s.objects.size();i++)	{
		if(s.objects[i].Model_Type=="XML") {
			XMLModel xmlmodel;
			xmlmodel.Load(s.objects[i].Model_Path);
			xmlmodel.name=s.objects[i].Model_Name;
			xmlmodel.scale=s.objects[i].Model_Size.x;
			models_xml.push_back(xmlmodel);
		}
	}
	return 0;
}

bool doRotate = false;
int xMouse = 0;
int yMouse = 0;

// listener orientation
float xRotation = 0.0f;
float yRotation = -90.0f;

// keyboard control
bool moveForward = false;
bool moveBackward = false;
bool moveLeft = false;
bool moveRight = false;
bool moveUp = false;
bool moveDown = false;

float xListenerPos = 30.0f;
float yListenerPos = 1.3f;
float zListenerPos = 0.4f;

void mouseFunc(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			doRotate = true;
			xMouse = x;
			yMouse = y;
		}
		else
		if (state == GLUT_UP) {
			doRotate = false;
		}
		break;

	default:
		break;
	}
}
void motionFunc(int x, int y) {
	int dx = x - xMouse;
	int dy = y - yMouse;

	// view rotation about y-axis
	yRotation += (float)dx * 0.5f;
	if (yRotation > 180.0f)
		yRotation -= 360.0f;
	else
	if (yRotation < -180.0f)
		yRotation += 360.0f;

	// view rotation about x-axis
	const float xExtent = 88.0f;
	xRotation += (float)dy * 0.5f;
	if (xRotation > xExtent)
		xRotation = xExtent;
	else 
	if (xRotation < -xExtent)
		xRotation = -xExtent;

	xMouse = x;
	yMouse = y;
}

void doListenerMovement() {
	moveForward=gKeys['w'];
	moveBackward=gKeys['s'];
	moveLeft=gKeys['a'];
	moveRight=gKeys['d'];
	moveUp=gKeys['q'];
	moveDown=gKeys['z'];

	// Update user movement
	const float MOVEMENT_SPEED = 0.3f;
	float forward = 0.0f;

	if (moveForward)
		forward += MOVEMENT_SPEED;
	if (moveBackward)
		forward -= MOVEMENT_SPEED;

	float right = 0.0f;
	if (moveLeft)
		right -= MOVEMENT_SPEED;
	if (moveRight)
		right += MOVEMENT_SPEED;	

	float up = 0.0f;
    if (moveUp)
        up += MOVEMENT_SPEED;
    if (moveDown)
        up -= MOVEMENT_SPEED;
	
	float xRight = (float)cos(yRotation * (PI / 180.0f));
	float yRight = 0.0f;
	float zRight = (float)sin(yRotation * (PI / 180.0f));

	xListenerPos += xRight * right;
	yListenerPos += yRight * right;
	zListenerPos += zRight * right;

	float xForward = (float)sin(yRotation * (PI / 180.0f)) * cos(xRotation  * (PI / 180.0f));
	float yForward = -(float)sin(xRotation  * (PI / 180.0f));
	float zForward = -(float)cos(yRotation * (PI / 180.0f)) * cos(xRotation  * (PI / 180.0f));

	xListenerPos += xForward * forward;
	yListenerPos += yForward * forward;
	zListenerPos += zForward * forward;

	yListenerPos += up;

	if (yListenerPos < 1.0f) {
		yListenerPos = 1.0f;
    }

	if(gKeys['j'])yRotation-=MOVEMENT_SPEED *200* (PI / 180.0f);
	if(gKeys['l'])yRotation+=MOVEMENT_SPEED *200* (PI / 180.0f);

}

#endif
