// Open source code used. Better for texture mapping

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <math.h>			// Header File For The Math Library
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include "glext.h"
#include "objects.h"
#include "stdlib.h"
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

float points[45][45][3];    // The Array For The Points On The Grid Of Our "Wave"
int wiggle_count = 0;		// Counter Used To Control How Fast Flag Waves

GLfloat	xrot;				// X Rotation ( NEW )
GLfloat	yrot;				// Y Rotation ( NEW )
GLfloat	zrot;				// Z Rotation ( NEW )
GLfloat hold;				// Temporarily Holds A Floating Point Value

GLfloat xpos=0,zpos=0;

GLuint	filter=2;				// Which Filter To Use

static GLuint	ROOM;	
table *Table[5];
wall *Wall[10];


typedef struct												// Create A Structure
{
	GLubyte	*imageData;										// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel.
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
} TextureImage;												// Structure Name

GLuint	texture[18];

TextureImage textures[2];

const int MAX_Cells = 512;

typedef struct tagVERTEX
{
	float x, y, z;
	float u, v;
} VERTEX;

typedef struct tagTRIANGLE
{
	VERTEX vertex[3];
} TRIANGLE;

typedef struct tagSECTOR
{
	int numtriangles;
	TRIANGLE* triangle;
} SECTOR;

SECTOR sector1;				// Our Model Goes Here:


GLdouble XP=0;
GLdouble YP=0;
GLdouble ZP=0;

GLdouble sceneroty;
GLdouble heading;
GLdouble _heading = 0;
GLdouble walkbias = 0;
GLdouble walkbiasangle = 0;
GLdouble zprot;

GLdouble XPOS = (GLdouble)(MAX_Cells/2);
GLdouble ZPOS = (GLdouble)(MAX_Cells/2);
const GLdouble piover180 = 0.0174532925f;

GLdouble	mouse_x, mouse_y;
GLdouble	mouse_3d_x, mouse_3d_y;
int mouse_wrap_x = 0;
int mouse_wrap_y = 0;
bool invert_mouse = false;  // Used to invert the y-axis for the mouse
GLdouble sensitivity = 8;  // the higher the #, the lower mouse the sensitivity.

bool jump_flag=0;

int InitGL(GLvoid);
int DrawGLScene(GLvoid);


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



int LoadGLTextures(char *file,GLuint *texture)									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP(file))
	{
		Status=TRUE;									// Set The Status To TRUE

        glGenTextures(3, &texture[0]);          // Create Three Textures

				// Create Nearest Filtered Texture
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

                // Create Linear Filtered Texture
            glBindTexture(GL_TEXTURE_2D, texture[1]);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

				// Create MipMapped Texture
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        
	}

	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	return Status;										// Return The Status
}


int LoadGLTextures()									// Load Bitmap And Convert To A Texture
{
  int Status=FALSE;								// Status Indicator
  AUX_RGBImageRec *TextureImage[1];				// Create Storage Space For The Textures
  memset(TextureImage,0,sizeof(void *)*1);		// Set The Pointer To NULL

  if (TextureImage[0]=LoadBMP("texture/floor.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[1]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }
  

  if (TextureImage[0]=LoadBMP("texture/wall.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[2]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/clay.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[3]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/LightMapFloor.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[4]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/LightMapCeiling.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[5]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/LightMapBackWall.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[6]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/LightMapFrontWall.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[7]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/LightMapLeftWall.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[8]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/LightMapRightWall.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[9]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/clouds.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[10]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/m16.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[11]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/shock_rifle.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[12]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/shock_rifle_blast.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[13]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/shock_rifle_blast2.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[14]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[14]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/crosshair.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[15]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[15]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/smoke.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[16]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[16]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0]=LoadBMP("texture/ceiling.bmp"))	// Load Particle Texture
  {
    Status=TRUE;								// Set The Status To TRUE
    glGenTextures(1, &texture[17]);				// Create One Texture

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[17]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);           
  }

  if (TextureImage[0])							// If Texture Exists
	{
    if (TextureImage[0]->data)					// If Texture Image Exists
    {
      free(TextureImage[0]->data);			// Free The Texture Image Memory
    }
    free(TextureImage[0]);						// Free The Image Structure
  }

  return Status;									// Return The Status
}


void readstr(FILE *f,char *string) {
	do
	{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}
